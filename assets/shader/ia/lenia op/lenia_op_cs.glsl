layout (local_size_x = X_THREADS, local_size_y = Y_THREADS, local_size_z = 1) in;

layout (binding = COUNTER_BIND, std430) buffer CounterBlock { Counter data_[]; };

layout (binding = CURR_IMG_BIND, rgba8) writeonly uniform image2D current_image;
layout (binding = PREV_IMG_BIND, rgba8) readonly uniform image2D prev_image;

uniform int u_radius;
uniform float u_dt;
uniform float u_mu;
uniform float u_sigma;
uniform float u_rho;
uniform float u_omega;

vec2 Convolution(ivec2 coords)
{
  float sum = 0;
  float total = 0;
  int total_lines = TOTAL_LINES(u_radius);
  for(int i = 0; i < total_lines; i++)
  {
    int index = ARRAY_3D_INDEX(coords.x, coords.y, i, C_HEIGHT, MAX_RADIUS);
    sum += data_[index].live_;
    total += data_[index].count_;
  }
  return vec2(sum, total);
}

void main() 
{
  // Obtener el color previo
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

  vec2 conv = Convolution(texelCoord);

  float avg = conv.x / conv.y;

  float growth = (GaussBell(avg, u_mu, u_sigma) * 2.0) - 1.0;

  float value = imageLoad(prev_image, texelCoord).a;

  float c = clamp(value + (1.0 / u_dt) * growth, 0.0, 1.0);

  imageStore(current_image, texelCoord, vec4(1.0, 1.0, 1.0, c));
}
