#version 460 core

#define COUNTER_BIND 0
#define INDICES_BIND 1
#define CURR_IMG_BIND 2
#define PREV_IMG_BIND 3

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout (binding = CURR_IMG_BIND, rgba8) writeonly uniform image2D current_image;
layout (binding = PREV_IMG_BIND, rgba8) readonly uniform image2D prev_image;

uniform float u_radius;
uniform float u_dt;
uniform float u_mu;
uniform float u_sigma;
uniform float u_rho;
uniform float u_omega;

float GaussBell(float x, float m, float s){ return exp(-(x - m) * (x - m) / s / s / 2.0); }

vec2 Convolution(ivec2 coords)
{
  float sum = 0;
  float total = 0;
  for(int x = -int(u_radius); x <= int(u_radius); x++)
  {
    for(int y = -int(u_radius); y <= int(u_radius); y++)
    {
      vec2 neighbord_texel = (coords + ivec2(x,y));
      float alpha =  imageLoad(prev_image, ivec2(neighbord_texel)).a;

      float norm_rad = sqrt(float(x * x + y * y)) / u_radius;
      float weight = GaussBell(norm_rad, u_rho, u_omega);
      
      sum += (alpha * weight);
      total += weight;
    }
  }
  return vec2(sum, total);
}

void main() 
{
  // Obtener el color previo
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
  vec4 currentColor = imageLoad(prev_image, texelCoord);

  vec2 conv = Convolution(texelCoord);

  float avg = conv.x / conv.y;

  float growth = (GaussBell(avg, u_mu, u_sigma) * 2.0) - 1.0;

  float value = imageLoad(prev_image, texelCoord).a;

  float c = clamp(value + (1.0 / u_dt) * growth, 0.0, 1.0);

  imageStore(current_image, texelCoord, vec4(1.0, 1.0, 1.0, c));
}
