#include <cstdlib>
#include <glog/logging.h>

#include <time.h>
#include <cstdlib>
#include <uv.h>

#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include "mcc/gui/gui.h"

#include "mcc/mcc.h"
#include "mcc/font/font.h"
#include "mcc/window.h"
#include "mcc/render_loop.h"


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

  glfwSetErrorCallback(&mcc::OnGlfwError);
  LOG_IF(FATAL, !glfwInit()) << "error initializing GLFW";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
  glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);

  using namespace mcc;
  font::Initialize();
  Entities::Initialize();
  const auto loop = uv_loop_new();
  Engine::Init(loop);
  Engine::OnPostInit([]() {
    Mouse::Initialize(Window::GetHandle());
  });
  RenderLoop::Initialize(loop);
  gui::Screen::Init();
  Renderer::Init();
  camera::PerspectiveCameraBehavior::Init();
  mesh::Mesh::Init();
  Window::Init();
  Systems::Init();
  terrain::Terrain::Init();

  Engine::Run();
  return EXIT_SUCCESS;
}