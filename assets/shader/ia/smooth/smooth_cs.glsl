#version 460 core

#define COUNTER_BIND 0
#define INDICES_BIND 1
#define CURR_IMG_BIND 2
#define PREV_IMG_BIND 3

#define SECTORS 4

#define O_RADIUS 12.0
#define I_RADIUS 1.44

#define NEAR_NEIGHBORS 6

#define C_WIDTH 512
#define C_HEIGHT 512
#define C_DEPTH (NEAR_NEIGHBORS + int((O_RADIUS * SECTORS)))

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

struct Counter
{
  float live_;
  uint count_;
};

layout (binding = COUNTER_BIND, std430) buffer CounterBlock { Counter data_[]; };
layout (binding = INDICES_BIND, std430) buffer IndicesBlock { vec2 indices_[]; };

layout (binding = CURR_IMG_BIND, rgba8) writeonly uniform image2D current_image;
layout (binding = PREV_IMG_BIND, rgba8) readonly uniform image2D prev_image;

int Array3DIndex(int x, int y, int z, int max_y, int max_z) { return (x * max_y * max_z) + (y * max_z) + (z); }

vec2 SumNeighbors(int col, int row, int for_start, int for_end)
{
  float sum_life = 0.0;
  float total_count = 0.0;

  for(int z = for_start; z < for_end; z += 2)
  {
    int index = Array3DIndex(col, row, z, C_HEIGHT, C_DEPTH);

    ivec2 start_coord = ivec2(indices_[index]);
    ivec2 end_coord = ivec2(indices_[index + 1]);

    Counter start_counter = data_[start_coord.x + start_coord.y * C_WIDTH];
    Counter end_counter = data_[end_coord.x + end_coord.y * C_WIDTH];

    Counter counter = Counter(end_counter.live_ - start_counter.live_, end_counter.count_ - start_counter.count_);
    
    sum_life += counter.live_;
    total_count += counter.count_;
  }

  return vec2(sum_life, total_count);
}

float getAlpha(int col, int row)
{
  vec2 far = SumNeighbors(col, row, NEAR_NEIGHBORS, C_DEPTH);
  vec2 near = SumNeighbors(col, row, 0, NEAR_NEIGHBORS);

  far -= near;

  float far_div = far.x / far.y;
  float near_div = near.x / near.y;

  float ret_alpha = 0.0;

  if (near_div >= 0.5 && 0.26 <= far_div && far_div <= 0.46)
    ret_alpha = 1.0;

  if (near_div < 0.5 && 0.27 <= far_div && far_div <= 0.36)
    ret_alpha = 1.0;

  return ret_alpha;
 }

void main() 
{
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
  vec4 currentColor = imageLoad(prev_image, texelCoord);

  vec4 updatedColor = vec4(currentColor.rgb, getAlpha(texelCoord.x, texelCoord.y));

  imageStore(current_image, texelCoord, updatedColor);
}