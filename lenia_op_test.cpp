#include <iostream>
#include <cassert>

#define MAX_RADIUS 20

#define C_WIDTH 512
#define C_HEIGHT 512
#define TOTAL_LINES(rad) (static_cast<unsigned>((rad * 2) + 1))
#define TOTAL_COLUMNS(rad) (static_cast<unsigned>((rad * 2) + 1))

#define ARRAY_3D_INDEX(x, y, z, max_y, max_z)                                              \
  static_cast<unsigned>(x) * static_cast<unsigned>(max_y) * static_cast<unsigned>(max_z) + \
      static_cast<unsigned>(y) * static_cast<unsigned>(max_z) +                            \
      static_cast<unsigned>(z)

#define ARRAY_2D_INDEX(x, y, max_x)                         \
  static_cast<unsigned>(y) * static_cast<unsigned>(max_x) + \
      static_cast<unsigned>(x)

#define GaussBell(x, m, s) (exp(-(x - m) * (x - m) / s / s / 2.0))

typedef struct
{
  float x, y;
} Vec2;

Vec2 operator+(const Vec2 &v, const Vec2 &v1) { return Vec2{v.x + v1.x, v.y + v1.y}; }

typedef struct
{
  float x, y, z;
} Vec3;

typedef struct
{
  float r, g, b, a;
} Vec4;

bool operator==(const Vec4 &v, const Vec4 &v1) { return (v.r == v1.r && v.g == v1.g && v.b == v1.b && v.a == v1.a); }

typedef struct
{
  float live_, count_;
} Counter;

int u_radius = 15;
float u_dt = 5.0f;
float u_mu = 0.14f;
float u_sigma = 0.014f;
float u_rho = 0.5f;
float u_omega = 0.15f;

Vec4 prev_image[C_WIDTH * C_HEIGHT];
Vec4 original_image[C_WIDTH * C_HEIGHT];
Vec4 divided_image[C_WIDTH * C_HEIGHT];
Counter data[C_WIDTH * C_HEIGHT * MAX_RADIUS];

float clamp(float value, float min_value, float max_value)
{
  if (value < min_value)
    return min_value;
  if (value > max_value)
    return max_value;
  return value;
}

float EuclidianDistance(float x, float y) { return sqrt(x * x + y * y); }

void InitPrevImage()
{
  float alive = 255.0f;
  for (unsigned i = 0; i < C_WIDTH * C_HEIGHT; i++)
    prev_image[i] = Vec4{alive, alive, alive, (float)(rand() % 255)};
}

Vec2 OriginalConvolution(Vec2 coords)
{
  float sum = 0;
  float total = 0;
  for (int x = -int(u_radius); x <= int(u_radius); x++)
  {
    for (int y = -int(u_radius); y <= int(u_radius); y++)
    {
      Vec2 neighbord_texel = (coords + Vec2{(float)(x), (float)(y)});

      if (neighbord_texel.y < 0)
        neighbord_texel.y = (C_HEIGHT - neighbord_texel.y);
      if (neighbord_texel.y >= C_HEIGHT)
        neighbord_texel.y -= C_HEIGHT;
      if (neighbord_texel.x < 0)
        neighbord_texel.x = (C_HEIGHT - neighbord_texel.x);
      if (neighbord_texel.x >= C_HEIGHT)
        neighbord_texel.x -= C_HEIGHT;

      int neighbor_index = ARRAY_2D_INDEX(neighbord_texel.x, neighbord_texel.y, C_WIDTH);
      float neighbor_alpha = prev_image[neighbor_index].a;

      float norm_rad = EuclidianDistance(x, y) / u_radius;
      float weight = GaussBell(norm_rad, u_rho, u_omega);

      sum += (neighbor_alpha * weight);
      total += weight;
    }
  }
  return Vec2{sum, total};
}

void OriginalLenia()
{
  for (unsigned y = 0; y < C_HEIGHT; y++)
  {
    for (unsigned x = 0; x < C_WIDTH; x++)
    {
      // Obtener el color previo
      int index = ARRAY_2D_INDEX(x, y, C_WIDTH);
      Vec4 prev_color = prev_image[index];

      Vec2 conv = OriginalConvolution(Vec2{(float)(x), (float)(y)});

      float avg = conv.x / conv.y;

      float growth = (GaussBell(avg, u_mu, u_sigma) * 2.0) - 1.0;

      float value = prev_color.a;

      float c = clamp(value + (1.0 / u_dt) * growth, 0.0, 1.0);

      original_image[index].a = c;
    }
  }
}

void Step1()
{
  for (unsigned z = 0; z < u_radius; z++)
  {
    for (unsigned y = 0; y < C_HEIGHT; y++)
    {
      for (unsigned x = 0; x < C_WIDTH; x++)
      {
        Vec3 gid = Vec3{(float)(x), (float)(y), (float)(z)};

        int local_y = (gid.z - u_radius);
        int neighbour_y = (local_y + gid.y);
        if (neighbour_y < 0)
          neighbour_y = (C_HEIGHT - neighbour_y);
        if (neighbour_y >= C_HEIGHT)
          neighbour_y -= C_HEIGHT;

        int total_columns = TOTAL_COLUMNS(u_radius);

        float sum = 0.0;
        float total = 0.0;
        for (int local_x = -u_radius; local_x <= u_radius; local_x++)
        {
          int neighbour_x = (local_x + gid.x);
          if (neighbour_x < 0)
            neighbour_x = (C_WIDTH - neighbour_x);
          if (neighbour_x >= C_WIDTH)
            neighbour_x -= C_WIDTH;

          int neighbor_index = ARRAY_2D_INDEX(neighbour_x, neighbour_y, C_WIDTH);

          float alpha = prev_image[neighbor_index].a;

          float norm_rad = EuclidianDistance(local_x, local_y) / u_radius;
          float weight = GaussBell(norm_rad, u_rho, u_omega);

          sum += (alpha * weight);
          total += weight;
        }

        int index = ARRAY_3D_INDEX(gid.x, gid.y, gid.z, C_HEIGHT, MAX_RADIUS);
        data[index] = Counter{sum, total};
      }
    }
  }
}

Vec2 DividedConvolution(Vec2 coords)
{
  float sum = 0;
  float total = 0;
  int total_lines = TOTAL_LINES(u_radius);
  for (unsigned i = 0; i < total_lines; i++)
  {
    int index = ARRAY_3D_INDEX(coords.x, coords.y, i, C_HEIGHT, MAX_RADIUS);
    sum += data[index].live_;
    total += data[index].count_;
  }
  return Vec2{sum, total};
}

void Step2()
{
  for (unsigned y = 0; y < C_HEIGHT; y++)
  {
    for (unsigned x = 0; x < C_HEIGHT; x++)
    {
      // Obtener el color previo
      int current_index = ARRAY_2D_INDEX(x, y, C_WIDTH);

      Vec2 conv = DividedConvolution(Vec2{(float)(x), (float)(y)});

      float avg = conv.x / conv.y;

      float growth = (GaussBell(avg, u_mu, u_sigma) * 2.0) - 1.0;

      float value = prev_image[current_index].a;

      float c = clamp(value + (1.0 / u_dt) * growth, 0.0, 1.0);

      divided_image[current_index].a = c;
    }
  }
}

void DividedLenia()
{
  Step1();
  Step2();
}

int main(int, char **)
{
#ifdef _WIN32
  system("cls");
#elif __linux__
  system("clear");
#else
#error "Sistema operativo no compatible"
#endif
  srand(time(NULL));
  InitPrevImage();

  OriginalLenia();
  DividedLenia();
  for (unsigned i = 0; i < C_WIDTH * C_HEIGHT; i++)
  {
    bool check = (original_image[i] == divided_image[i]);
    if (!check)
      fprintf(stderr, "Failed in %d", i);
    assert(check);
  }

  fprintf(stdout, "All correct");
  return 0;
}