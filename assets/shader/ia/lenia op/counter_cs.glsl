layout (local_size_x = X_THREADS, local_size_y = Y_THREADS, local_size_z = 1) in;

layout (binding = PREV_IMG_BIND, rgba8) readonly uniform image2D prev_image;
layout (binding = CURR_IMG_BIND, rgba8) writeonly uniform image2D current_image;

layout (binding = COUNTER_BIND, std430) buffer CounterBlock { Counter data_[]; };

uniform int u_radius;
uniform float u_dt;
uniform float u_mu;
uniform float u_sigma;
uniform float u_rho;
uniform float u_omega;

void main() 
{
  ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);

  int local_y = (gid.z - u_radius);
  int neighbour_y = (local_y + gid.y);
  if (neighbour_y < 0)
    neighbour_y = (C_HEIGHT + neighbour_y);
  if (neighbour_y >= C_HEIGHT)
    neighbour_y -= C_HEIGHT;

  int total_columns = TOTAL_COLUMNS(u_radius);

  float sum = 0.0;
  float total = 0.0;
  for (int local_x = -u_radius; local_x <= u_radius; local_x++)
  {
    int neighbour_x = (local_x + gid.x);
    if (neighbour_x < 0)
      neighbour_x = (C_WIDTH + neighbour_x);
    if (neighbour_x >= C_WIDTH)
      neighbour_x -= C_WIDTH;

    float neighbour_alpha = imageLoad(prev_image, ivec2(neighbour_x, neighbour_y)).a;

    float norm_rad = EuclidianDistance(local_x, local_y) / u_radius;
    float weight = GaussBell(norm_rad, u_rho, u_omega);
    
    sum += (neighbour_alpha * weight);
    total += weight;
  }
  
  int index = ARRAY_3D_INDEX(gid.x, gid.y, gid.z, C_HEIGHT, MAX_RADIUS);
  data_[index] = Counter(sum, total);
}