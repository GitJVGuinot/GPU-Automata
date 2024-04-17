#version 460 core

#define COUNTER_BIND 0
#define INDICES_BIND 1
#define CURR_IMG_BIND 2
#define PREV_IMG_BIND 3

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout (binding = CURR_IMG_BIND, rgba8) writeonly uniform image2D current_image;
layout (binding = PREV_IMG_BIND, rgba8) readonly uniform image2D prev_image;

void main() 
{
  // Obtener el color previo
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
  vec4 currentColor = imageLoad(prev_image, texelCoord);

  // Obtener el componente alpha del pixel actual
  float alpha = currentColor.a;

  // Definir las reglas del Juego de la Vida de Conway
  float numAliveNeighbors = 0;

  for (int i = -1; i <= 1; i++) 
  {
    for (int j = -1; j <= 1; j++) 
    {
      ivec2 neighborCoord = texelCoord + ivec2(i, j);
      vec4 neighborColor = imageLoad(prev_image, neighborCoord);

      // Sumar el componente alpha del vecino actual si está vivo
      numAliveNeighbors += neighborColor.a;
    }
  }

  // Restar el propio componente alpha si está vivo
  numAliveNeighbors -= alpha;

  // Aplicar las reglas del Juego de la Vida
  if (alpha > 0.5) 
  {
    // Celula viva
    if (numAliveNeighbors < 2.0 || numAliveNeighbors > 3.0) 
    {
      // Celula muere por baja población o sobrepoblación
      alpha = 0.0;
    }
  } else 
  {
    // Celula muerta
    if (numAliveNeighbors == 3.0) 
    {
      // Celula nace por reproducción
      alpha = 1.0;
    }
  }

  // Mantener el color previo y actualizar el componente alpha
  vec4 updatedColor = vec4(currentColor.rgb, alpha);

  // Escribir el color actualizado en la imagen actual
  imageStore(current_image, texelCoord, updatedColor);
}