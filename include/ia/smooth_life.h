#include "engine/engine.h"

#ifndef __SMOOTH_LIFE_H__
#define __SMOOTH_LIFE_H__ 1

class SmoothLife
{
public:
  SmoothLife();
  void init(Math::Vec2 win);
  ~SmoothLife();

  void update();
  void imgui();

  void reset();
  void clean();

  u32 currentTexture();

private:
  void compileShaders();
  void swap();

  TimeCont update_timer_;
  u32 loops_;

  u32 pre_compute_program_, compute_program_;

  u32 width_, height_, depth_;
  f32 outter_rad_, inner_rad_;

  u32 counter_ssbo_, counter_indices_ssbo_;
  u32 prev_data_id_, current_data_id_;
};

#endif /* __SMOOTH_LIFE_H__ */
