#include <engine/jam_engine.h>
#include "ia/ia.h"

static f32 win_x = 512;
static f32 win_y = 512;

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

static int mode = 0;
static Conway conway;
static SmoothLife smooth_life;
static Lenia lenia;

void ChangeMode(int &mode, int signess, int min, int max)
{
  mode += signess;

  if (mode > max)
    mode = min;
  if (mode < min)
    mode = max;

  fprintf(stdout, "Mode: %d", mode);
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
  conway.init(Math::Vec2(win_x, win_y));
  smooth_life.init(Math::Vec2(win_x, win_y));
  lenia.init(Math::Vec2(win_x, win_y));

  Transform tr;
  tr.scale(Math::Vec3(1.0f));
  tr.rotate(Math::Vec3(Math::MathUtils::AngleToRads(90.0f), 0.0f, 0.0f));
  EM->newEntity("Quad");
  EM->setComponent(EM->getId("Quad"), img);
  EM->setComponent(EM->getId("Quad"), quad);
  EM->setComponent(EM->getId("Quad"), tr);
}

void UserUpdate(void *)
{
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
  }

  if (JAM_Engine::InputDown(Inputs::Key::Key_Left))
    ChangeMode(mode, -1, 0, 2);
  if (JAM_Engine::InputDown(Inputs::Key::Key_Right))
    ChangeMode(mode, 1, 0, 2);
}

void UserClean(void *) {}

s32 main(s32 argc, byte *argv[])
{
  JAM_Engine::Config config = {argc, argv, static_cast<s32>(win_x), static_cast<s32>(win_y), false, false, true};
  JAM_Engine::Init(UserInit, config);
  JAM_Engine::Update(UserUpdate);
  JAM_Engine::Clean(UserClean);

  return 0;
}
