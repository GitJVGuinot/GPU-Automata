#include "ia/conway.h"
#include "ia/gpu_helper.h"
#include "ia/defines.h"

Conway::Conway() {}

void Conway::init(Math::Vec2 win)
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

  glUseProgram(compute_program_);

  reset();
}

Conway::~Conway() {}

void Conway::swap()
{
  std::swap(current_data_id_, prev_data_id_);
}

void Conway::update()
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

  glUseProgram(0);
  /////////////////////////////////////////////////////////////////////////////

  glFinish();
  update_timer_.stopTime();
}

void Conway::imgui()
{
  ImGui::Begin("GPU Automata");

  ImGui::Text("Type - Conway");
  ImGui::Text("Update time: %ld mcs", update_timer_.getElapsedTime(TimeCont::Precision::microseconds));
  ImGui::Text("Generation: %d", loops_);

  ImGui::End();
}

void Conway::reset()
{
  loops_ = 0;
  u_byte *data = reinterpret_cast<u_byte *>(std::calloc(width_ * height_ * 4, sizeof(u_byte)));

  if (!data)
    return;

  u_byte alive = 255;
  u_byte dead = 0;

  for (u32 i = 0; i < width_ * height_ * 4; i += 4)
  {
    data[i + 0] = alive;
    data[i + 1] = alive;
    data[i + 2] = alive;

    data[i + 3] = (rand() % 5 < 2) ? alive : dead;
  }

  glBindTexture(GL_TEXTURE_2D, current_data_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, prev_data_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, 0);

  DESTROY(data);
}

void Conway::clean()
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

u32 Conway::currentTexture() { return current_data_id_; }

void Conway::compileShaders()
{
  // Compute shader
  /////////////////////////////////////////////////////////////////////////////
  std::string conway_string = defines + LoadSourceFromFile(SHADER("ia/conway/conway_cs.glsl"));
  const char *conway_cs = conway_string.c_str();
  GLuint compute_shader = GPUHelper::CompileShader(GL_COMPUTE_SHADER, conway_cs, "conway shader");
  compute_program_ = GPUHelper::CreateProgram(compute_shader, "conway program");
  /////////////////////////////////////////////////////////////////////////////
}
