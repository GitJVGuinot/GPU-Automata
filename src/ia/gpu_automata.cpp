#include "ia/gpu_automata.h"

#define SECTORS 4

#define O_RADIUS 12.0f
#define I_RADIUS 1.44f

#define NEAR_NEIGHBORS 6.0f

#define DEPTH (static_cast<s32>(NEAR_NEIGHBORS + (O_RADIUS * SECTORS)))

#define ARRAY_3D_INDEX(x, y, z, max_y, max_z)                               \
  static_cast<u32>(x) * static_cast<u32>(max_y) * static_cast<u32>(max_z) + \
      static_cast<u32>(y) * static_cast<u32>(max_z) +                       \
      static_cast<u32>(z)

#define COUNTER_BIND 0
#define INDICES_BIND 1
#define CURR_IMG_BIND 2
#define PREV_IMG_BIND 3

void CheckComputeResults(GLuint counter_ssbo, GLuint prev_data_id, u32 width, u32 height)
{
  // Use glGetNamedBufferSubData to retrieve data from the buffer for debugging
  GPU_Automata::Counter *data = reinterpret_cast<GPU_Automata::Counter *>(std::calloc(width * height, sizeof(GPU_Automata::Counter)));
  glGetNamedBufferSubData(counter_ssbo, 0, width * height * sizeof(GPU_Automata::Counter), data);

  // Use glGetTexImage to retrieve data from the image for debugging
  u_byte *prev_image_data = reinterpret_cast<u_byte *>(std::calloc(width * height * 4, sizeof(u_byte)));
  glBindTexture(GL_TEXTURE_2D, prev_data_id);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, prev_image_data);

  DESTROY(data);
  DESTROY(prev_image_data);
}

GLuint CreateTexture(u32 width, u32 height, u_byte *data)
{
  GLuint id;
  // Generate texture
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  // Unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);
  return id;
}

GPU_Automata::GPU_Automata(Type type) : type_(type) {}

void GPU_Automata::init(Math::Vec2 win)
{
  width_ = static_cast<u32>(win.x);
  height_ = static_cast<u32>(win.y);
  outter_rad_ = O_RADIUS;
  inner_rad_ = I_RADIUS;
  depth_ = DEPTH;

  u_byte *data = reinterpret_cast<u_byte *>(std::calloc(width_ * height_ * 4, sizeof(u_byte)));

  if (!data)
  {
    width_ = 0;
    height_ = 0;

    return;
  }

  current_data_id_ = CreateTexture(width_, height_, data);
  prev_data_id_ = CreateTexture(width_, height_, data);

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

  this->reset();
}

GPU_Automata::~GPU_Automata() {}

void GPU_Automata::swap()
{
  std::swap(current_data_id_, prev_data_id_);
}

void GPU_Automata::update()
{
  update_timer_.startTime();
  this->swap();
  GLenum error = GL_NO_ERROR;

  if (type_ == Type::k_SmoothLife)
  {
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
  }

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

  if (type_ == Type::k_Lenia || type_ == Type::k_Optimized_Lenia)
  {
    glUniform1f(glGetUniformLocation(compute_program_, "u_radius"), radius_);
    glUniform1f(glGetUniformLocation(compute_program_, "u_dt"), dt_);
    glUniform1f(glGetUniformLocation(compute_program_, "u_mu"), mu_);
    glUniform1f(glGetUniformLocation(compute_program_, "u_sigma"), sigma_);
    glUniform1f(glGetUniformLocation(compute_program_, "u_rho"), rho_);
    glUniform1f(glGetUniformLocation(compute_program_, "u_omega"), omega_);
  }

  glUseProgram(0);
  /////////////////////////////////////////////////////////////////////////////

  update_timer_.stopTime();
}

const byte *TakeAutomataTypeString(GPU_Automata::Type type)
{
  switch (type)
  {
  case GPU_Automata::Type::k_Conways:
    return "Conways";
  case GPU_Automata::Type::k_SmoothLife:
    return "Smooth life";
  case GPU_Automata::Type::k_Lenia:
    return "Basic Lenia";
  case GPU_Automata::Type::k_Optimized_Lenia:
    return "Optimized Lenia";
  default:
    return "GPU_Automata::Type not valid";
  }
}

void GPU_Automata::imgui()
{
  ImGui::Begin("GPU Automata");

  ImGui::Text("Update time: %ld mcs", update_timer_.getElapsedTime(TimeCont::Precision::microseconds));
  ImGui::Text("Type - %s", TakeAutomataTypeString(type_));

  if (type_ == Type::k_Lenia || type_ == Type::k_Optimized_Lenia)
  {
    ImGui::SliderFloat("Radius", &radius_, 10.0f, 25.0f);
    ImGui::SliderFloat("Delta Time", &dt_, 5.0f, 15.0f);
    ImGui::SliderFloat("Mu", &mu_, 0.14f, 0.7f);
    ImGui::SliderFloat("Sigma", &sigma_, 0.014f, 0.07f);
    ImGui::SliderFloat("Rho", &rho_, 0.025f, 0.075f);
    ImGui::SliderFloat("Omega", &omega_, 0.05f, 0.025f);
  }

  ImGui::End();
}

void GPU_Automata::reset()
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

    if (type_ == Type::k_Conways || type_ == Type::k_SmoothLife)
      data[i + 3] = (rand() % 5 < 2) ? alive : dead;

    if (type_ == Type::k_Lenia || type_ == Type::k_Optimized_Lenia)
      data[i + 3] = static_cast<u_byte>(rand() % 255);
  }

  glBindTexture(GL_TEXTURE_2D, current_data_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, prev_data_id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, 0);

  DESTROY(data);
}

void GPU_Automata::clean()
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

u32 GPU_Automata::currentTexture() { return current_data_id_; }

void GPU_Automata::compileShaders()
{
  GLint success;

  // Pre Compute shader
  /////////////////////////////////////////////////////////////////////////////
  GLuint preComputeShader = glCreateShader(GL_COMPUTE_SHADER);

  std::string pre_compute = LoadSourceFromFile(SHADER("ia/smooth/counter_cs.glsl"));
  const char *pre_compute_cs = pre_compute.c_str();
  glShaderSource(preComputeShader, 1, &pre_compute_cs, nullptr);

  glCompileShader(preComputeShader);

  glGetShaderiv(preComputeShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar info_log[512];
    glGetShaderInfoLog(preComputeShader, 512, NULL, info_log);
    fprintf(stderr, "Error compiling pre compute shader:\n%s\n", info_log);
    std::exit(-1);
  }
  /////////////////////////////////////////////////////////////////////////////

  // Pre Compute program link
  /////////////////////////////////////////////////////////////////////////////
  pre_compute_program_ = glCreateProgram();
  glAttachShader(pre_compute_program_, preComputeShader);
  glLinkProgram(pre_compute_program_);

  glGetProgramiv(pre_compute_program_, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLchar info_log[512];
    glGetProgramInfoLog(pre_compute_program_, 512, NULL, info_log);
    fprintf(stderr, "Error linking pre compute shaders:\n%s\n", info_log);
    std::exit(-1);
  }
  /////////////////////////////////////////////////////////////////////////////

  // Compute shader
  /////////////////////////////////////////////////////////////////////////////
  GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);

  if (type_ == Type::k_Conways)
  {
    std::string conway_string = LoadSourceFromFile(SHADER("ia/conway/conway_cs.glsl"));
    const char *conway_cs = conway_string.c_str();
    glShaderSource(computeShader, 1, &conway_cs, nullptr);
  }

  if (type_ == Type::k_SmoothLife)
  {
    std::string smooth_string = LoadSourceFromFile(SHADER("ia/smooth/smooth_cs.glsl"));
    const char *smooth_cs = smooth_string.c_str();
    glShaderSource(computeShader, 1, &smooth_cs, nullptr);
  }

  if (type_ == Type::k_Lenia)
  {
    std::string lenia_string = LoadSourceFromFile(SHADER("ia/lenia/lenia_cs.glsl"));
    const char *lenia_cs = lenia_string.c_str();
    glShaderSource(computeShader, 1, &lenia_cs, nullptr);
  }

  if (type_ == Type::k_Optimized_Lenia)
  {
    std::string lenia_string = LoadSourceFromFile(SHADER("ia/lenia op/optimized_lenia_cs.glsl"));
    const char* lenia_cs = lenia_string.c_str();
    glShaderSource(computeShader, 1, &lenia_cs, nullptr);
  }

  glCompileShader(computeShader);

  glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar info_log[512];
    glGetShaderInfoLog(computeShader, 512, NULL, info_log);
    fprintf(stderr, "Error compiling compute shader:\n%s\n", info_log);
    std::exit(-1);
  }
  /////////////////////////////////////////////////////////////////////////////

  // Compute program link
  /////////////////////////////////////////////////////////////////////////////
  compute_program_ = glCreateProgram();
  glAttachShader(compute_program_, computeShader);
  glLinkProgram(compute_program_);

  glGetProgramiv(compute_program_, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLchar info_log[512];
    glGetProgramInfoLog(compute_program_, 512, NULL, info_log);
    fprintf(stderr, "Error linking compute shaders:\n%s\n", info_log);
    std::exit(-1);
  }
  /////////////////////////////////////////////////////////////////////////////
}
