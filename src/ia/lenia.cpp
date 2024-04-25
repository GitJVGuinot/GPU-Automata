#include "ia/lenia.h"
#include "ia/gpu_helper.h"
#include "ia/defines.h"

Lenia::Lenia() {}

void Lenia::init(Math::Vec2 win)
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

  // Default Lenia config
  radius_ = 15.0f;
  dt_ = 5.0f;
  mu_ = 0.14f;
  sigma_ = 0.014f;
  rho_ = 0.5f;
  omega_ = 0.15f;

  glUseProgram(compute_program_);

  reset();
}

Lenia::~Lenia() {}

void Lenia::swap()
{
  std::swap(current_data_id_, prev_data_id_);
}

void Lenia::update()
{
  update_timer_.startTime();
  loops_++;

  swap();
  
  GLenum error = GL_NO_ERROR;

  // GPU Automata
  /////////////////////////////////////////////////////////////////////////////
  glUseProgram(compute_program_);

  glBindImageTexture(CURR_IMG_BIND, current_data_id_, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
  glBindImageTexture(PREV_IMG_BIND, prev_data_id_, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

  // Dispatch Compute Shader with appropriate workgroup sizes
  glDispatchCompute(width_, height_, 1);
  error = glGetError();
  if (error != GL_NO_ERROR)
    fprintf(stderr, "Compute Shader Dispatch Error: %d\n", error);

  glMemoryBarrier(GL_ALL_BARRIER_BITS);

  glUniform1f(glGetUniformLocation(compute_program_, "u_radius"), radius_);
  glUniform1f(glGetUniformLocation(compute_program_, "u_dt"), dt_);
  glUniform1f(glGetUniformLocation(compute_program_, "u_mu"), mu_);
  glUniform1f(glGetUniformLocation(compute_program_, "u_sigma"), sigma_);
  glUniform1f(glGetUniformLocation(compute_program_, "u_rho"), rho_);
  glUniform1f(glGetUniformLocation(compute_program_, "u_omega"), omega_);

  glUseProgram(0);
  /////////////////////////////////////////////////////////////////////////////

  glFinish();
  update_timer_.stopTime();
}

void Lenia::imgui()
{
  ImGui::Begin("GPU Automata");

  ImGui::Text("Type - Lenia");
  ImGui::Text("Update time: %ld ms", update_timer_.getElapsedTime(TimeCont::Precision::milliseconds));
  ImGui::Text("Generation: %d", loops_);

  ImGui::SliderFloat("Radius", &radius_, 10.0f, 25.0f);
  ImGui::SliderFloat("Delta Time", &dt_, 5.0f, 15.0f);
  ImGui::SliderFloat("Mu", &mu_, 0.14f, 0.7f);
  ImGui::SliderFloat("Sigma", &sigma_, 0.014f, 0.07f);
  ImGui::SliderFloat("Rho", &rho_, 0.025f, 0.075f);
  ImGui::SliderFloat("Omega", &omega_, 0.05f, 0.025f);

  ImGui::End();
}

void Lenia::reset()
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

void Lenia::clean()
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

u32 Lenia::currentTexture() { return current_data_id_; }

void Lenia::compileShaders()
{
  // Compute shader
  /////////////////////////////////////////////////////////////////////////////
  std::string lenia_string = defines + LoadSourceFromFile(SHADER("ia/lenia/lenia_cs.glsl"));
  const char *lenia_cs = lenia_string.c_str();

  GLuint compute_shader = GPUHelper::CompileShader(GL_COMPUTE_SHADER, lenia_cs, "lenia shader");
  compute_program_ = GPUHelper::CreateProgram(compute_shader, "lenia program");
  /////////////////////////////////////////////////////////////////////////////
}
