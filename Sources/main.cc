#include <cstdlib>
#include <glog/logging.h>

#include <time.h>
#include <cstdlib>
#include <uv.h>

#include "mcc/mcc.h"
#include "mcc/font/font.h"
#include "mcc/window.h"
#include "mcc/render_loop.h"
#include "mcc/scene.h"

#include "mcc/camera/perspective_camera.h"

#include "mcc/engine/engine.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/renderer/renderer.h"

#include "mcc/terrain/terrain.h"

int main(int argc, char** argv) {
  srand(time(NULL));
  ::google::InitGoogleLogging(argv[0]);
  ::google::LogToStderr();
  ::google::ParseCommandLineFlags(&argc, &argv, true);

  using namespace mcc;
  font::Initialize();
  Entities::Initialize();
  const auto loop = uv_loop_new();
  Engine::Init(loop);
  Engine::OnPostInit([]() {
    Mouse::Initialize(Window::GetHandle());
  });
  RenderLoop::Initialize(loop);
  Renderer::Init();
  camera::PerspectiveCameraBehavior::Init();
  Window::Init();
  Systems::Init();
  terrain::Terrain::Init();

  Engine::Run();
  return EXIT_SUCCESS;
}