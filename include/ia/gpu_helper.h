#include "engine/engine.h"

#ifndef __GPU_HELPER_H__
#define __GPU_HELPER_H__ 1

class GPUHelper
{
public:
  static u32 CreateTexture(u32 width, u32 height, u_byte *data);
  static u32 CompileShader(u32 shader_type, const byte *source, const char *name);
  static u32 CreateProgram(u32 compute_shader, const char *name);

private:
  GPUHelper();
  ~GPUHelper();
};

#endif /* __GPU_HELPER_H__ */