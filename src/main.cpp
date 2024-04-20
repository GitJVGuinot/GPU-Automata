#include <engine/jam_engine.h>
#include "ia/ia.h"

static f32 win_x = C_WIDTH * SCALAR_SIZE;
static f32 win_y = C_HEIGHT * SCALAR_SIZE;

static const Camera::CamConfig config = {
    .camera_render_type_ = Camera::RenderType::Orthographic,
    .light_render_type_ = Camera::LightRenderType::Forward,
    .cam_win_ = Math::Vec2(win_x, win_y),
    .pos_ = Math::Vec3(0.0f, 0.0f, 1.0f),
    .target_ = Math::Vec3::zero,
    .right_ = 0.5f,
    .left_ = -0.5f,
    .top_ = 0.5f,
    .bottom_ = -0.5f,
    .GetMesh = &JAM_Engine::GetMesh,
    .UploadMesh = &JAM_Engine::UploadCustomMesh,
    .WheelScroll = &JAM_Engine::WheelScroll,
    .MousePosition = &JAM_Engine::MousePosition,
    .KeyInputPress = &JAM_Engine::InputPress,
    .MouseInputPress = &JAM_Engine::InputPress};

static Camera camera;

static Mesh *quad = nullptr;
static Material *img = nullptr;

const static s32 max_modes = 3;
static s32 mode = 3;
static Conway conway;
static SmoothLife smooth_life;
static Lenia lenia;
static LeniaOp lenia_op;

void ChangeMode(s32 &mode, s32 signess, s32 min, s32 max)
{
  mode += signess;

  if (mode > max)
    mode = min;
  if (mode < min)
    mode = max;

  fprintf(stdout, "Mode: %d\n", mode);
}

void UserInit(s32 argc, byte *argv[], void *)
{
  PRINT_ARGS;
  camera.init(config);

  // Material
  img = JAM_Engine::GetMaterial(JAM_Engine::UploadMaterial(SHADER("ia/render/image.fs"), SHADER("ia/render/image.vs")));

  // Mesh
  quad = JAM_Engine::GetMesh(Mesh::Platonic::k_Quad);

  // GPU Automata
  conway.init(Math::Vec2(C_WIDTH, C_HEIGHT));
  smooth_life.init(Math::Vec2(C_WIDTH, C_HEIGHT));
  lenia.init(Math::Vec2(C_WIDTH, C_HEIGHT));
  lenia_op.init(Math::Vec2(C_WIDTH, C_HEIGHT));

  Transform tr;
  tr.scale(Math::Vec3(1.0f));
  tr.rotate(Math::Vec3(Math::MathUtils::AngleToRads(90.0f), 0.0f, 0.0f));
  EM->newEntity("Quad");
  EM->setComponent(EM->getId("Quad"), img);
  EM->setComponent(EM->getId("Quad"), quad);
  EM->setComponent(EM->getId("Quad"), tr);
}

static s32 frames = -1;

void UserUpdate(void *)
{
  frames++;

  u32 texture_id;
  if (mode == 0)
  {
    conway.update();
    conway.imgui();
    texture_id = conway.currentTexture();
  }

  if (mode == 1)
  {
    smooth_life.update();
    smooth_life.imgui();
    texture_id = smooth_life.currentTexture();
  }

  if (mode == 2)
  {
    lenia.update();
    lenia.imgui();
    texture_id = lenia.currentTexture();
  }

  if (mode == 3)
  {
    lenia_op.update();
    lenia_op.imgui();
    texture_id = lenia_op.currentTexture();
  }

  if (JAM_Engine::InputDown(Inputs::Key::Key_F5))
    JAM_Engine::RechargeShaders();

  JAM_Engine::BeginRender(&camera);

  img->use();
  img->setTexture("Image", texture_id, 0);
  JAM_Engine::Render("Quad");

  JAM_Engine::EndRender();

  if (JAM_Engine::InputDown(Inputs::Key::Key_R))
  {
    if (mode == 0)
      conway.reset();
    if (mode == 1)
      smooth_life.reset();
    if (mode == 2)
      lenia.reset();
    if (mode == 3)
      lenia_op.reset();
  }

  if (JAM_Engine::InputDown(Inputs::Key::Key_Left))
    ChangeMode(mode, -1, 0, max_modes);
  if (JAM_Engine::InputDown(Inputs::Key::Key_Right))
    ChangeMode(mode, 1, 0, max_modes);
}

void UserClean(void *) {}

void Test();

s32 main(s32 argc, byte *argv[])
{
  Test();
  return 0;
  JAM_Engine::Config config = {argc, argv, static_cast<s32>(win_x), static_cast<s32>(win_y), false, false, true};
  JAM_Engine::Init(UserInit, config);
  JAM_Engine::Update(UserUpdate);
  JAM_Engine::Clean(UserClean);

  return 0;
}

s32 u_radius = 15;
f32 u_dt = 5.0f;
f32 u_mu = 0.14f;
f32 u_sigma = 0.014f;
f32 u_rho = 0.5f;
f32 u_omega = 0.15f;

Math::Vec4 prev_image[C_WIDTH * C_HEIGHT];
Math::Vec4 original_image[C_WIDTH * C_HEIGHT];
Math::Vec4 divided_imgage[C_WIDTH * C_HEIGHT];
Counter data[C_WIDTH * C_HEIGHT * MAX_RADIUS];

void InitPrevImage()
{
  u_byte alive = 255;
  for (u32 i = 0; i < C_WIDTH * C_HEIGHT; i++)
    prev_image[i] = Math::Vec4(alive, alive, alive, rand() % alive);
}

Math::Vec2 OriginalConvolution(Math::Vec2 coords)
{
  f32 sum = 0;
  f32 total = 0;
  for (s32 x = -s32(u_radius); x <= s32(u_radius); x++)
  {
    for (s32 y = -s32(u_radius); y <= s32(u_radius); y++)
    {
      Math::Vec2 neighbord_texel = (coords + Math::Vec2(x, y));
      if (neighbord_texel.y < 0)
        neighbord_texel.y = (C_HEIGHT - neighbord_texel.y);
      if (neighbord_texel.y >= C_HEIGHT)
        neighbord_texel.y -= C_HEIGHT;
      if (neighbord_texel.x < 0)
        neighbord_texel.x = (C_HEIGHT - neighbord_texel.x);
      if (neighbord_texel.x >= C_HEIGHT)
        neighbord_texel.x -= C_HEIGHT;

      s32 neighbor_index = ARRAY_2D_INDEX(neighbord_texel.x, neighbord_texel.y, C_WIDTH);
      f32 neighbor_alpha = prev_image[neighbor_index].w;

      f32 norm_rad = sqrt(f32(x * x + y * y)) / u_radius;
      f32 weight = GaussBell(norm_rad, u_rho, u_omega);

      sum += (neighbor_alpha * weight);
      total += weight;
    }
  }
  return Math::Vec2(sum, total);
}

void OriginalLenia()
{
  for (u32 y = 0; y < C_HEIGHT; y++)
  {
    for (u32 x = 0; x < C_WIDTH; x++)
    {
      // Obtener el color previo
      s32 index = ARRAY_2D_INDEX(x, y, C_WIDTH);
      Math::Vec4 prev_color = prev_image[index];

      Math::Vec2 conv = OriginalConvolution(Math::Vec2(x, y));

      f32 avg = conv.x / conv.y;

      f32 growth = (GaussBell(avg, u_mu, u_sigma) * 2.0) - 1.0;

      f32 value = prev_color.w;

      f32 c = std::clamp(value + (1.0 / u_dt) * growth, 0.0, 1.0);

      original_image[index].w = c;
    }
  }
}

void Step1()
{
  for (u32 z = 0; z < u_radius; z++)
  {
    for (u32 y = 0; y < C_HEIGHT; y++)
    {
      for (u32 x = 0; x < C_WIDTH; x++)
      {
        Math::Vec3 gid = Math::Vec3(x, y, z);

        s32 local_y = (gid.z - u_radius);
        s32 neighbour_y = (local_y + gid.y);
        if (neighbour_y < 0)
          neighbour_y = (C_HEIGHT - neighbour_y);
        if (neighbour_y >= C_HEIGHT)
          neighbour_y -= C_HEIGHT;

        s32 total_columns = TOTAL_COLUMNS(u_radius);

        f32 sum = 0.0;
        f32 total = 0.0;
        for (s32 local_x = -u_radius; local_x <= u_radius; local_x++)
        {
          s32 neighbour_x = (local_x + gid.x);
          if (neighbour_x < 0)
            neighbour_x = (C_HEIGHT - neighbour_x);
          if (neighbour_x >= C_HEIGHT)
            neighbour_x -= C_HEIGHT;

          s32 neighbor_index = ARRAY_2D_INDEX(neighbour_x, neighbour_y, C_WIDTH);

          f32 alpha = prev_image[neighbor_index].w;

          // f32 dist = distance(vec2(gid.xy), vec2(neighbor)) / u_radius;
          f32 norm_rad = sqrt(f32(local_x * local_x + local_y * local_y)) / u_radius;
          f32 weight = GaussBell(norm_rad, u_rho, u_omega);

          sum += (alpha * weight);
          total += weight;
        }

        s32 index = ARRAY_3D_INDEX(gid.x, gid.y, gid.z, C_HEIGHT, MAX_RADIUS);
        data[index] = Counter(sum, total);
      }
    }
  }
}

Math::Vec2 DividedConvolution(Math::Vec2 coords)
{
  f32 sum = 0;
  f32 total = 0;
  s32 total_lines = TOTAL_LINES(u_radius);
  for (u32 i = 0; i < total_lines; i++)
  {
    s32 index = ARRAY_3D_INDEX(coords.x, coords.y, i, C_HEIGHT, MAX_RADIUS);
    sum += data[index].live_;
    total += data[index].count_;
  }
  return Math::Vec2(sum, total);
}

void Step2()
{
  for (u32 y = 0; y < C_HEIGHT; y++)
  {
    for (u32 x = 0; x < C_HEIGHT; x++)
    {
      // Obtener el color previo
      s32 current_index = ARRAY_2D_INDEX(x, y, C_WIDTH);

      Math::Vec2 conv = DividedConvolution(Math::Vec2(x, y));

      f32 avg = conv.x / conv.y;

      f32 growth = (GaussBell(avg, u_mu, u_sigma) * 2.0) - 1.0;

      f32 value = prev_image[current_index].w;

      f32 c = std::clamp(value + (1.0 / u_dt) * growth, 0.0, 1.0);

      divided_imgage[current_index].w = c;
    }
  }
}

void DividedLenia()
{
  Step1();
  Step2();
}

void Test()
{
  InitPrevImage();
  OriginalLenia();
  DividedLenia();
  for (u32 i = 0; i < C_WIDTH * C_HEIGHT; i++)
    assert(original_image[i] == divided_imgage[i]);
}