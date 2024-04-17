#include "engine/engine.h"

#ifndef __LENIA_H__
#define __LENIA_H__ 1

class Lenia
{
public:
  Lenia();
  void init(Math::Vec2 win);
  ~Lenia();

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

  TimeCont update_timer_;

  u32 pre_compute_program_, compute_program_;

  u32 width_, height_;

  u32 prev_data_id_, current_data_id_;
};

#endif /* __LENIA_H__ */
