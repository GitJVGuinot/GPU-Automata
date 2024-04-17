uniform sampler2D Image;

void main() 
{
  vec4 texture_color = texture(Image, vertex_data.tex_coord);
  Draw(texture_color);
}
