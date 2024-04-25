#ifndef __IA_DEFINES_H__
#define __IA_DEFINES_H__

#define SCALAR_SIZE 4.0f // Only to render

#define PREV_IMG_BIND 0
#define CURR_IMG_BIND 1
#define COUNTER_BIND 2
#define INDICES_BIND 3

#define SECTORS 4

#define MAX_RADIUS 20
#define O_RADIUS 12.0f
#define I_RADIUS 1.44f

#define NEAR_NEIGHBORS 6

#define C_WIDTH 512
#define C_HEIGHT 512
#define C_DEPTH (static_cast<s32>(NEAR_NEIGHBORS + (O_RADIUS * SECTORS)))
#define TOTAL_LINES(rad) (static_cast<u32>((rad * 2) + 1))
#define TOTAL_COLUMNS(rad) (static_cast<u32>((rad * 2) + 1))

#define ARRAY_3D_INDEX(x, y, z, max_y, max_z)                               \
  static_cast<u32>(x) * static_cast<u32>(max_y) * static_cast<u32>(max_z) + \
      static_cast<u32>(y) * static_cast<u32>(max_z) +                       \
      static_cast<u32>(z)

#define ARRAY_2D_INDEX(x, y, max_x)               \
  static_cast<u32>(y) * static_cast<u32>(max_x) + \
      static_cast<u32>(x)

struct Counter
{
  f32 live_;
  f32 count_;
};

#define GaussBell(x, m, s) (expf(-(x - m) * (x - m) / s / s / 2.0f))
#define EuclidianDistance(x, y) (sqrtf(x * x + y * y))


const char defines[] = R"(
#version 460

#define PREV_IMG_BIND 0
#define CURR_IMG_BIND 1
#define COUNTER_BIND 2
#define INDICES_BIND 3

#define SECTORS 4

#define MAX_RADIUS 20
#define O_RADIUS 12.0
#define I_RADIUS 1.44

#define NEAR_NEIGHBORS 6

#define C_WIDTH 512
#define C_HEIGHT 512
#define C_DEPTH int(NEAR_NEIGHBORS + (O_RADIUS * SECTORS))
#define TOTAL_LINES(rad) ((rad * 2) + 1)
#define TOTAL_COLUMNS(rad) ((rad * 2) + 1)

#define ARRAY_3D_INDEX(x, y, z, max_y, max_z) int((x *  max_y * max_z) + (y * max_z) + (z))
#define ARRAY_2D_INDEX(x, y, max_x) int((y * max_x) + (x))

struct Counter
{
  float live_;
  float count_;
};

#define GaussBell(x, m, s) (exp(-(x - m) * (x - m) / s / s / 2.0f))
#define EuclidianDistance(x, y) (sqrt(x * x + y * y))
)";

#endif /* __IA_DEFINES_H__ */