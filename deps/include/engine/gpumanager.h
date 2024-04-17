#include "engine.h"

#ifndef __GPUMANAGER_H__
#define __GPUMANAGER_H__ 1

/**
 * @class GPUResources
 */
class GPUResources
{
public:
  /**
   * @brief Instance a resource of the gpu.
   */
  static GPUResources *Instance();

  /**
   * @brief Free all resources
  */
 void free();

  /**
   * @brief Create a shader program and link it.
   *
   * @param fragmentSource Fragment shader source code.
   * @param vertexSource Vertex shader source code.
   *
   * @return Shader program identifier.
   */
  u32 CreateProgram(const byte *fragmentSource = nullptr, const byte *vertexSource = nullptr);

  /**
   * @brief Compile a shader.
   *
   * @param shaderType Shader type.
   * @param source Shader source code.
   *
   * @return Identifier of the compiled shader.
   */
  u32 CompileShader(u32 shaderType, const byte *source);

  /**
   * @brief Create a vertex array.
   *
   * @param count Count the total array.
   *
   * @return Identifier of the vertex array.
   */
  u32 *CreateVertexArrays(u32 count);

  u32 *CreateBuffers(u32 count);

  u32 *CreateFrameBuffers(u32 count);

  u32 *CreateRenderBuffers(u32 count);

  u32 *CreateTextures(u32 count);

private:
  GPUResources();
  ~GPUResources();

  u32 *programs_;
  u32 programs_size_;

  u32 **vaos_;
  u32 *vaos_sizes_;
  u32 vaos_length_;

  u32 **bos_;
  u32 *bos_sizes_;
  u32 bos_length_;

  u32 **fbos_;
  u32 *fbos_sizes_;
  u32 fbos_length_;

  u32 **rbos_;
  u32 *rbos_sizes_;
  u32 rbos_length_;

  u32 **textures_;
  u32 *textures_sizes_;
  u32 textures_length_;
};

/**
 * @brief Access the GPU Manager intance (GPUResources)
 */
#define GPU (GPUResources::Instance())

#endif /* __GPUMANAGER_H__ */