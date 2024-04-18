layout (local_size_x = 1, local_size_y = C_WIDTH, local_size_z = 1) in;

layout (binding = COUNTER_BIND, std430) buffer CounterBlock { Counter data_[]; };
layout (binding = INDICES_BIND, std430) buffer IndicesBlock { vec2 indices_[]; };

layout (binding = CURR_IMG_BIND, rgba8) writeonly uniform image2D current_image;
layout (binding = PREV_IMG_BIND, rgba8) readonly uniform image2D prev_image;

void main() 
{
  int index_y = int(gl_GlobalInvocationID.y);

  float sum_live = 0;
  uint sum_counter = 0;

  // Acceder a todos los elementos en la dimensión x de cada linea de prev_image y Counter
  for (int index_x = 0; index_x < C_WIDTH; index_x++)
  {
    vec4 pixel_value = imageLoad(prev_image, ivec2(index_x, index_y));

    sum_live += pixel_value.a;
    sum_counter++;

    data_[index_y * C_WIDTH + index_x].live_ = sum_live;
    data_[index_y * C_WIDTH + index_x].count_ = sum_counter;
  }
}
