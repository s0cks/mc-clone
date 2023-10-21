#include <cstdlib>
#include <glog/logging.h>

#include <time.h>
#include <cstdlib>
#include <uv.h>

#define GL_SILENCE_DEPRECATION 1
#include "mcc/mcc.h"
#include "mcc/font/font.h"
#include "mcc/window.h"
#include "mcc/render_loop.h"
#include "mcc/scene.h"

#include "mcc/engine/engine.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/renderer/renderer.h"

int main(int argc, char** argv) {
  srand(time(NULL));
  ::google::InitGoogleLogging(argv[0]);
  ::google::LogToStderr();
  ::google::ParseCommandLineFlags(&argc, &argv, true);

  using namespace mcc;
  Entities::Initialize();
  const auto loop = uv_loop_new();
  RenderLoop::Initialize(loop);
  Components::Register<Renderable>();

  const auto engine = new Engine(loop);
  const auto renderer = Systems::Register<Renderer>();
  engine->Register(renderer);
  {
    Signature sig;
    sig.set(Components::GetComponentIdForType<Renderable>());
    Systems::SetSignature<Renderer>(sig);
  }
  font::Initialize();

  auto window = Window::Initialize(renderer, { 512, 512 });
  window->Open();
  return EXIT_SUCCESS;
}