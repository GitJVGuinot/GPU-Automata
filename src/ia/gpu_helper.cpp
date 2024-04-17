#include "ia/gpu_helper.h"

GLuint GPUHelper::CreateTexture(u32 width, u32 height, u_byte *data)
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

GLuint GPUHelper::CompileShader(u32 shader_type, const char *source, const char *name)
{
  GLint success;
  GLuint shader = glCreateShader(shader_type);

  glShaderSource(shader, 1, &source, nullptr);

  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar info_log[512];
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    fprintf(stderr, "Error compiling %s:\n%s\n", name, info_log);
    std::exit(-1);
  }
  return shader;
}

GLuint GPUHelper::CreateProgram(u32 compute_shader, const char *name)
{
  GLint success;
  GLuint program = glCreateProgram();
  glAttachShader(program, compute_shader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLchar info_log[512];
    glGetProgramInfoLog(program, 512, NULL, info_log);
    fprintf(stderr, "Error linking %s:\n%s\n", name, info_log);
    std::exit(-1);
  }
  return program;
}