#include "ia/lenia_op.h"
#include "ia/gpu_helper.h"
#include "ia/defines.h"

LeniaOp::LeniaOp() {}

void CheckComputeResults(GLuint counter_ssbo, GLuint prev_data_id, u32 width, u32 depth, u32 height)
{
  // Use glGetNamedBufferSubData to retrieve data from the buffer for debugging
  Counter* data = reinterpret_cast<Counter*>(std::calloc(width * height * depth, sizeof(Counter)));
  glGetNamedBufferSubData(counter_ssbo, 0, width * height * depth * sizeof(Counter), data);

  // Use glGetTexImage to retrieve data from the image for debugging
  u_byte* prev_image_data = reinterpret_cast<u_byte*>(std::calloc(width * height * 4, sizeof(u_byte)));
  glBindTexture(GL_TEXTURE_2D, prev_data_id);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, prev_image_data);

  DESTROY(data);
  DESTROY(prev_image_data);
}

void LeniaOp::init(Math::Vec2 win)
{
  loops_ = 0;
  width_ = static_cast<u32>(win.x);
  height_ = static_cast<u32>(win.y);

  u_byte *data = reinterpret_cast<u_byte *>(std::calloc(width_ * height_ * 4, sizeof(u_byte)));

  if (!data)
  {
    width_ = 0;
    height_ = 0;

    return;
  }

  current_data_id_ = GPUHelper::CreateTexture(width_, height_, data);
  prev_data_id_ = GPUHelper::CreateTexture(width_, height_, data);

  DESTROY(data);

  compileShaders();

  // Default LeniaOp config
  radius_ = 15;
  dt_ = 5.0f;
  mu_ = 0.14f;
  sigma_ = 0.014f;
  rho_ = 0.5f;
  omega_ = 0.15f;

  glUseProgram(compute_program_);

  // Counter
  /////////////////////////////////////////////////////////////////////////////
  glGenBuffers(1, &counter_ssbo_);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, counter_ssbo_);
  glBufferData(GL_SHADER_STORAGE_BUFFER, width_ * height_ * TOTAL_LINES(MAX_RADIUS) * sizeof(Counter), nullptr, GL_DYNAMIC_COPY);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COUNTER_BIND, counter_ssbo_);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  /////////////////////////////////////////////////////////////////////////////

  reset();
}

LeniaOp::~LeniaOp() {}

void LeniaOp::swap()
{
  std::swap(current_data_id_, prev_data_id_);
}

void LeniaOp::update()
{
  update_timer_.startTime();
  loops_++;

  swap();
  
  GLenum error = GL_NO_ERROR;

  // GPU Counter
  /////////////////////////////////////////////////////////////////////////////
  glUseProgram(pre_compute_program_);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COUNTER_BIND, counter_ssbo_);
  glBindImageTexture(CURR_IMG_BIND, current_data_id_, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
  glBindImageTexture(PREV_IMG_BIND, prev_data_id_, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

  glDispatchCompute(width_, height_, TOTAL_LINES(radius_));
  error = glGetError();
  if (error != GL_NO_ERROR)
    fprintf(stderr, "Compute Shader Dispatch Error: %d\n", error);

  glMemoryBarrier(GL_ALL_BARRIER_BITS);
  glUseProgram(0);
  /////////////////////////////////////////////////////////////////////////////

  // GPU Automata
  /////////////////////////////////////////////////////////////////////////////
  glUseProgram(compute_program_);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COUNTER_BIND, counter_ssbo_);
  glBindImageTexture(CURR_IMG_BIND, current_data_id_, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
  glBindImageTexture(PREV_IMG_BIND, prev_data_id_, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

  // Dispatch Compute Shader with appropriate workgroup sizes
  glDispatchCompute(width_, height_, 1);
  error = glGetError();
  if (error != GL_NO_ERROR)
    fprintf(stderr, "Compute Shader Dispatch Error: %d\n", error);

  glMemoryBarrier(GL_ALL_BARRIER_BITS);

  glUniform1i(glGetUniformLocation(compute_program_, "u_radius"), radius_);
  glUniform1f(glGetUniformLocation(compute_program_, "u_dt"), dt_);
  glUniform1f(glGetUniformLocation(compute_program_, "u_mu"), mu_);
  glUniform1f(glGetUniformLocation(compute_program_, "u_sigma"), sigma_);
  glUniform1f(glGetUniformLocation(compute_program_, "u_rho"), rho_);
  glUniform1f(glGetUniformLocation(compute_program_, "u_omega"), omega_);

  glUseProgram(0);
  /////////////////////////////////////////////////////////////////////////////

  update_timer_.stopTime();
}

void LeniaOp::imgui()
{
  ImGui::Begin("GPU Automata");

  ImGui::Text("Type - Lenia optimized");
  ImGui::Text("Update time: %ld mcs", update_timer_.getElapsedTime(TimeCont::Precision::microseconds));
  ImGui::Text("Generation: %d", loops_);

  ImGui::SliderInt("Radius", &radius_, 10, MAX_RADIUS);
  ImGui::SliderFloat("Delta Time", &dt_, 5.0f, 15.0f);
  ImGui::SliderFloat("Mu", &mu_, 0.14f, 0.7f);
  ImGui::SliderFloat("Sigma", &sigma_, 0.014f, 0.07f);
  ImGui::SliderFloat("Rho", &rho_, 0.025f, 0.075f);
  ImGui::SliderFloat("Omega", &omega_, 0.05f, 0.025f);

  ImGui::End();
}

void LeniaOp::reset()
{
  loops_ = 0;
  u_byte *data = reinterpret_cast<u_byte *>(std::calloc(width_ * height_ * 4, sizeof(u_byte)));

  if (!data)
    return;

  u_byte alive = 255;

  for (u32 i = 0; i < width_ * height_ * 4; i += 4)
  {
    data[i + 0] = alive;
    data[i + 1] = alive;
    data[i + 2] = alive;

    data[i + 3] = static_cast<u_byte>(rand() % 255);
  }

  glBindTexture(GL_TEXTURE_2D, current_data_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, prev_data_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, 0);

  DESTROY(data);
}

void LeniaOp::clean()
{
  u_byte *data = reinterpret_cast<u_byte *>(std::calloc(width_ * height_ * 4, sizeof(u_byte)));

  if (!data)
    return;

  glBindTexture(GL_TEXTURE_2D, current_data_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, prev_data_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, 0);

  DESTROY(data);
}

u32 LeniaOp::currentTexture() { return current_data_id_; }

void LeniaOp::compileShaders()
{
  // Pre compute shader
  ///////////////////////////////////////////////////////////////////////////
  std::string pre_lenia_string = defines + LoadSourceFromFile(SHADER("ia/lenia op/counter_cs.glsl"));
  const char *pre_lenia_cs = pre_lenia_string.c_str();

  GLuint pre_compute_shader = GPUHelper::CompileShader(GL_COMPUTE_SHADER, pre_lenia_cs, "lenia counter shader");
  pre_compute_program_ = GPUHelper::CreateProgram(pre_compute_shader, "lenia counter program");
  ///////////////////////////////////////////////////////////////////////////

  // Compute shader
  /////////////////////////////////////////////////////////////////////////////
  std::string lenia_string = defines + LoadSourceFromFile(SHADER("ia/lenia op/lenia_op_cs.glsl"));
  const char *lenia_cs = lenia_string.c_str();
  GLuint compute_shader = GPUHelper::CompileShader(GL_COMPUTE_SHADER, lenia_cs, "lenia op shader");
  compute_program_ = GPUHelper::CreateProgram(compute_shader, "lenia op program");
  /////////////////////////////////////////////////////////////////////////////
}
