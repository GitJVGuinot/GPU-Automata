#include "engine/engine.h"

#ifndef __CONWAY_H__
#define __CONWAY_H__ 1

class Conway
{
public:
  Conway();
  void init(Math::Vec2 win);
  ~Conway();

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

  u32 compute_program_;

  u32 width_, height_;

  u32 prev_data_id_, current_data_id_;
};

#endif /* __CONWAY_H__ */
