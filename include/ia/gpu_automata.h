#include "engine/engine.h"

#ifndef __GPU_AUTOMATA_H__
#define __GPU_AUTOMATA_H__ 1

class GPU_Automata
{
public:
  enum class Type : s16
  {
    k_Conways = 0,
    k_SmoothLife,
    k_Lenia,
    k_Optimized_Lenia,
  };

  struct Counter
  {
    f32 live_;
    u32 count_;
  };

  GPU_Automata(Type type);
  void init(Math::Vec2 win);
  ~GPU_Automata();

  void update();
  void imgui();

  void reset();
  void clean();

  u32 currentTexture();

  float radius_;
  float dt_;
  float mu_;
  float sigma_;
  float rho_;
  float omega_;
  
private:
  void compileShaders();
  void swap();

  Type type_;

  TimeCont update_timer_;

  u32 pre_compute_program_, compute_program_;

  u32 width_, height_, depth_;
  f32 outter_rad_, inner_rad_;

  u32 counter_ssbo_, counter_indices_ssbo_;
  u32 prev_data_id_, current_data_id_;
};

#endif /* __GPU_AUTOMATA_H__ */
