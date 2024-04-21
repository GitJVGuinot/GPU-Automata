#include "engine/engine.h"
#include "defines.h"

#ifndef __LENIA_OP_H__
#define __LENIA_OP_H__ 1

class LeniaOp
{
public:
  LeniaOp();
  void init(Math::Vec2 win);
  ~LeniaOp();

  void update();
  void imgui();

  void reset();
  void clean();

  u32 currentTexture();

  s32 radius_;
  float dt_;
  float mu_;
  float sigma_;
  float rho_;
  float omega_;
  
private:
  struct Pixel
  {
    u_byte r, g, b, a;
  };
  float sumOriginal(Pixel* prev_img, u32 x, u32 y);
  float sumCounter(Counter* counter, u32 x, u32 y);
  void checkSingleSlot(Counter* counter, Pixel* prev_img, u32 x, u32 y);
  void checkComputeResults();
  void compileShaders();
  void swap();

  TimeCont update_timer_;
  u32 loops_;

  u32 counter_ssbo_;
  u32 pre_compute_program_, compute_program_;

  u32 width_, height_;

  u32 prev_data_id_, current_data_id_;
};

#endif /* __LENIA_OP_H__ */
