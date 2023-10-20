#include <cstdlib>
#include <glog/logging.h>

#include <uv.h>

#define GL_SILENCE_DEPRECATION 1
#include "mcc/mcc.h"
#include "mcc/font/font.h"
#include "mcc/window.h"
#include "mcc/render_loop.h"
#include "mcc/scene.h"

#include "mcc/ecs/coordinator.h"
#include "mcc/renderer/renderer.h"

int main(int argc, char** argv) {
  ::google::InitGoogleLogging(argv[0]);
  ::google::LogToStderr();
  ::google::ParseCommandLineFlags(&argc, &argv, true);

  using namespace mcc;

  const auto coord = Coordinator::Initialize();
  coord->RegisterComponent<Renderable>();

  const auto renderer = coord->RegisterSystem<Renderer>();
  {
    Signature sig;
    sig.set(coord->GetComponentId<Renderable>());
    coord->SetSystemSignature<Renderer>(sig);
  }

  const auto loop = uv_loop_new();
  RenderLoop::Initialize(loop);
  font::Initialize();

  auto window = Window::Initialize(renderer, { 512, 512 });
  window->Open();
  return EXIT_SUCCESS;
}