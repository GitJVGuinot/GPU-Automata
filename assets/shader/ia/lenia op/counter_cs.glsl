#line 1
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout (binding = COUNTER_BIND, std430) buffer CounterBlock { Counter data_[]; };

layout (binding = CURR_IMG_BIND, rgba8) writeonly uniform image2D current_image;
layout (binding = PREV_IMG_BIND, rgba8) readonly uniform image2D prev_image;

uniform int u_radius;
uniform float u_dt;
uniform float u_mu;
uniform float u_sigma;
uniform float u_rho;
uniform float u_omega;

void main() 
{
  ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);

  int current_y = (gid.z - u_radius);
  // if(current_y < 0)
  //   current_y = (C_HEIGHT - current_y);
  // if(current_y >= C_HEIGHT)
  //   current_y -= C_HEIGHT;

  int total_columns = TOTAL_COLUMNS(u_radius);

  float sum = 0.0;
  float total = 0.0;
  for(int x = 0; x < total_columns; x++)
  {
    int current_x = (gid.x - u_radius);
    // if(current_x < 0)
    //   current_x = (C_HEIGHT - current_x);
    // if(current_x >= C_HEIGHT)
    //   current_x -= C_HEIGHT;

    float dist = distance(vec2(gid.xy), vec2(current_x, current_y)) / u_radius;
    float weight = GaussBell(dist, u_rho, u_omega);
    float alpha =  imageLoad(prev_image, ivec2(current_x, gid.z)).a;

    sum += (alpha * weight);
    total += weight;
  }

  int index = ARRAY_3D_INDEX(gid.x, gid.y, gid.z, C_HEIGHT, MAX_RADIUS);
  data_[index].live_ = sum;
  data_[index].count_ = total;
}