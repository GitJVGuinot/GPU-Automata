#include "ia/smooth_life.h"
#include "ia/gpu_helper.h"
#include "ia/defines.h"

void CheckComputeResults(GLuint counter_ssbo, GLuint prev_data_id, u32 width, u32 height)
{
  // Use glGetNamedBufferSubData to retrieve data from the buffer for debugging
  SmoothLife::Counter *data = reinterpret_cast<SmoothLife::Counter *>(std::calloc(width * height, sizeof(SmoothLife::Counter)));
  glGetNamedBufferSubData(counter_ssbo, 0, width * height * sizeof(SmoothLife::Counter), data);

  // Use glGetTexImage to retrieve data from the image for debugging
  u_byte *prev_image_data = reinterpret_cast<u_byte *>(std::calloc(width * height * 4, sizeof(u_byte)));
  glBindTexture(GL_TEXTURE_2D, prev_data_id);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, prev_image_data);

  DESTROY(data);
  DESTROY(prev_image_data);
}

SmoothLife::SmoothLife() {}

void SmoothLife::init(Math::Vec2 win)
{
  width_ = static_cast<u32>(win.x);
  height_ = static_cast<u32>(win.y);
  outter_rad_ = O_RADIUS;
  inner_rad_ = I_RADIUS;
  depth_ = C_DEPTH;

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

  // Counter
  /////////////////////////////////////////////////////////////////////////////
  glGenBuffers(1, &counter_ssbo_);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, counter_ssbo_);
  glBufferData(GL_SHADER_STORAGE_BUFFER, width_ * height_ * sizeof(Counter), nullptr, GL_DYNAMIC_COPY);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COUNTER_BIND, counter_ssbo_);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  /////////////////////////////////////////////////////////////////////////////

  // Counter indices
  /////////////////////////////////////////////////////////////////////////////
  Math::Vec2 *indices = reinterpret_cast<Math::Vec2 *>(std::calloc(width_ * height_ * depth_, sizeof(Math::Vec2)));

  Math::Vec2 coords[static_cast<u32>(NEAR_NEIGHBORS)] = {
      Math::Vec2(-1.0f, -1.0f), Math::Vec2(+1.0f, -1.0f),
      Math::Vec2(-1.0f, +0.0f), Math::Vec2(+1.0f, +0.0f),
      Math::Vec2(-1.0f, +1.0f), Math::Vec2(+1.0f, +1.0f)};

  for (s32 col = 0; col < static_cast<s32>(width_); col++)
  {
    for (s32 row = 0; row < static_cast<s32>(height_); row++)
    {
      f32 y = -O_RADIUS;
      for (s32 depth = 0; depth < static_cast<s32>(depth_); depth += 2)
      {
        u32 index = ARRAY_3D_INDEX(col, row, depth, height_, depth_);

        Math::Vec2 start_coord;
        Math::Vec2 end_coord;

        if (depth < static_cast<s32>(NEAR_NEIGHBORS))
        {
          start_coord = coords[depth] + Math::Vec2(static_cast<f32>(col), static_cast<f32>(row));
          end_coord = coords[depth + 1] + Math::Vec2(static_cast<f32>(col), static_cast<f32>(row));
        }
        else
        {
          f32 x_offset = std::floor(sqrtf((outter_rad_ * outter_rad_) - (y * y)));
          start_coord = Math::Vec2(static_cast<f32>(col) - x_offset - 1.0f, static_cast<f32>(row) + y);
          end_coord = Math::Vec2(static_cast<f32>(col) + x_offset, static_cast<f32>(row) + y);
          y++;
        }

        start_coord.x = std::max(start_coord.x, 0.0f);
        start_coord.x = std::min(start_coord.x, static_cast<f32>(width_) - 1.0f);

        start_coord.y = std::max(start_coord.y, 0.0f);
        start_coord.y = std::min(start_coord.y, static_cast<f32>(height_) - 1.0f);

        end_coord.x = std::max(end_coord.x, 0.0f);
        end_coord.x = std::min(end_coord.x, static_cast<f32>(width_) - 1.0f);

        end_coord.y = std::max(end_coord.y, 0.0f);
        end_coord.y = std::min(end_coord.y, static_cast<f32>(height_) - 1.0f);

        indices[index] = start_coord;
        indices[index + 1] = end_coord;
      }
    }
  }

  glGenBuffers(1, &counter_indices_ssbo_);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, counter_indices_ssbo_);
  glBufferData(GL_SHADER_STORAGE_BUFFER, width_ * height_ * depth_ * sizeof(Math::Vec2), indices, GL_STATIC_COPY);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, INDICES_BIND, counter_indices_ssbo_);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  glUseProgram(0);

  DESTROY(indices);
  /////////////////////////////////////////////////////////////////////////////

  reset();
}

SmoothLife::~SmoothLife() {}

void SmoothLife::swap()
{
  std::swap(current_data_id_, prev_data_id_);
}

void SmoothLife::update()
{
  update_timer_.startTime();
  swap();
  GLenum error = GL_NO_ERROR;

  // GPU Counter
  /////////////////////////////////////////////////////////////////////////////
  glUseProgram(pre_compute_program_);

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COUNTER_BIND, counter_ssbo_);
  glBindImageTexture(CURR_IMG_BIND, current_data_id_, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
  glBindImageTexture(PREV_IMG_BIND, prev_data_id_, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

  // Dispatch Compute Shader with appropriate workgroup sizes
  glDispatchCompute(width_, 1, 1);
  error = glGetError();
  if (error != GL_NO_ERROR)
    fprintf(stderr, "Compute Shader Dispatch Error: %d\n", error);

  glMemoryBarrier(GL_ALL_BARRIER_BITS);
  // CheckComputeResults(counter_ssbo_, prev_data_id_, width_, height_);
  glUseProgram(0);
  /////////////////////////////////////////////////////////////////////////////

  // GPU Automata
  /////////////////////////////////////////////////////////////////////////////
  glUseProgram(compute_program_);

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, COUNTER_BIND, counter_ssbo_);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, INDICES_BIND, counter_indices_ssbo_);
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

  update_timer_.stopTime();
}

void SmoothLife::imgui()
{
  ImGui::Begin("GPU Automata");

  ImGui::Text("Update time: %ld mcs", update_timer_.getElapsedTime(TimeCont::Precision::microseconds));
  ImGui::Text("Type - Smooth life");

  ImGui::Text("Radius: %.1f", O_RADIUS);

  ImGui::End();
}

void SmoothLife::reset()
{
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

void SmoothLife::clean()
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

u32 SmoothLife::currentTexture() { return current_data_id_; }

void SmoothLife::compileShaders()
{
  // Pre Compute shader
  /////////////////////////////////////////////////////////////////////////////
  std::string pre_compute = defines + LoadSourceFromFile(SHADER("ia/smooth/counter_cs.glsl"));
  const char *pre_compute_cs = pre_compute.c_str();

  GLuint pre_compute_shader = GPUHelper::CompileShader(GL_COMPUTE_SHADER, pre_compute_cs, "pre smooth shader");
  pre_compute_program_ = GPUHelper::CreateProgram(pre_compute_shader, "pre smooth program");
  /////////////////////////////////////////////////////////////////////////////

  // Compute shader
  /////////////////////////////////////////////////////////////////////////////
  std::string smooth_string = defines + LoadSourceFromFile(SHADER("ia/smooth/smooth_cs.glsl"));
  const char *smooth_cs = smooth_string.c_str();

  GLuint compute_shader = GPUHelper::CompileShader(GL_COMPUTE_SHADER, smooth_cs, "smooth shader");
  compute_program_ = GPUHelper::CreateProgram(compute_shader, "smoot program");
  /////////////////////////////////////////////////////////////////////////////
}
