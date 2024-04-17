#include <engine/jam_engine.h>
#include "ia/gpu_automata.h"

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
  .MouseInputPress = &JAM_Engine::InputPress };

static Camera camera;

static Mesh* quad = nullptr;
static Material* img = nullptr;

static GPU_Automata automata(GPU_Automata::Type::k_Conways);

static TimeCont timer_;

void UserInit(s32 argc, byte* argv[], void*)
{
  PRINT_ARGS;
  camera.init(config);
  
  // Material
  img = JAM_Engine::GetMaterial(JAM_Engine::UploadMaterial(SHADER("ia/render/image.fs"), SHADER("ia/render/image.vs")));

  // Mesh
  quad = JAM_Engine::GetMesh(Mesh::Platonic::k_Quad);

  // GPU Automata
  automata.init(Math::Vec2(win_x, win_y));

  Transform tr;
  tr.scale(Math::Vec3(1.0f));
  tr.rotate(Math::Vec3(Math::MathUtils::AngleToRads(90.0f), 0.0f, 0.0f));
  EM->newEntity("Quad");
  EM->setComponent(EM->getId("Quad"), img);
  EM->setComponent(EM->getId("Quad"), quad);
  EM->setComponent(EM->getId("Quad"), tr);

  timer_.startTime();
  
}

void UserUpdate(void*)
{
  automata.update();
  automata.imgui();

  if (JAM_Engine::InputDown(Inputs::Key::Key_F5))
    JAM_Engine::RechargeShaders();

  JAM_Engine::BeginRender(&camera);
  
  img->use();
  img->setTexture("Image", automata.currentTexture(), 0);
  JAM_Engine::Render("Quad");
  
  JAM_Engine::EndRender();

  if (JAM_Engine::InputDown(Inputs::Key::Key_R))
    automata.reset();

  if(timer_.getElapsedTime(TimeCont::Precision::milliseconds) < 10)
    automata.reset();
}

void UserClean(void*) {}

s32 main(s32 argc, byte* argv[])
{
  JAM_Engine::Config config = { argc, argv, static_cast<s32>(win_x), static_cast<s32>(win_y), false, false, true };
  JAM_Engine::Init(UserInit, config);
  JAM_Engine::Update(UserUpdate);
  JAM_Engine::Clean(UserClean);

  return 0;
}
