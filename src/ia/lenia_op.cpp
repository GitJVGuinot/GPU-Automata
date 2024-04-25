#include "ia/lenia_op.h"
#include "ia/gpu_helper.h"

LeniaOp::LeniaOp() {}

float LeniaOp::sumOriginal(Pixel* prev_img, u32 x, u32 y)
{
  Counter sum = { 0.0f, 0.0f };

  for (s32 ny = -radius_; ny <= radius_; ny++)
  {
    for (s32 nx = -radius_; nx <= radius_; nx++)
    {
      Math::Vec2 neighbour = Math::Vec2(static_cast<float>(nx), static_cast<float>(ny)) + Math::Vec2(static_cast<float>(x), static_cast<float>(y));
      if (neighbour.y < 0)
        neighbour.y = (C_HEIGHT - neighbour.y);
      if (neighbour.y >= C_HEIGHT)
        neighbour.y -= C_HEIGHT;

      if (neighbour.x < 0)
        neighbour.x = (C_WIDTH - neighbour.x);
      if (neighbour.x >= C_WIDTH)
        neighbour.x -= C_WIDTH;

      float alpha = prev_img[ARRAY_2D_INDEX(neighbour.x, neighbour.y, C_WIDTH)].a;

      float norm_rad = sqrtf(float(x * x + y * y)) / static_cast<float>(radius_);
      float weight = GaussBell(norm_rad, rho_, omega_);

      sum.live_ += (alpha * weight);
      sum.count_ += weight;
    }
  }

  return sum.live_ / sum.count_;
}

float LeniaOp::sumCounter(Counter* counter, u32 x, u32 y)
{
  Counter sum = {0.0f, 0.0f};

  for (u32 i = 0; i < TOTAL_LINES(radius_); i++)
  {
    u32 index = ARRAY_3D_INDEX(x, y, i, C_HEIGHT, MAX_RADIUS);
    sum.live_ += counter[index].live_;
    sum.count_ += counter[index].count_;
  }

  return sum.live_ / sum.count_;
}

void LeniaOp::checkSingleSlot(Counter *counter, Pixel *prev_img, u32 x, u32 y)
{
  float sum_original = sumOriginal(prev_img, x, y);
  float sum_counter = sumCounter(counter, x, y);

  assert(sum_original == sum_counter);
}

void LeniaOp::checkComputeResults()
{
  // Use glGetNamedBufferSubData to retrieve data from the buffer for debugging
  Counter* data = reinterpret_cast<Counter*>(std::calloc(width_ * height_ * MAX_RADIUS, sizeof(Counter)));
  assert(data);
  glGetNamedBufferSubData(counter_ssbo_, 0, width_ * height_ * MAX_RADIUS * sizeof(Counter), data);

  // Use glGetTexImage to retrieve data from the image for debugging
  Pixel* prev_image_data = reinterpret_cast<Pixel*>(std::calloc(width_ * height_, sizeof(Pixel)));
  assert(prev_image_data);
  glBindTexture(GL_TEXTURE_2D, prev_data_id_);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, prev_image_data);

  for (u32 y = 0; y < C_WIDTH; y++) 
    for (u32 x = 0; x < C_HEIGHT; x++)
      checkSingleSlot(data, prev_image_data, x, y);

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
  reset();
  
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
  checkComputeResults();
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
