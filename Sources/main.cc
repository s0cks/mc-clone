#include <cstdlib>
#include <glog/logging.h>

#include <time.h>
#include <cstdlib>
#include <uv.h>

#include <units.h>

#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include "mcc/gui/gui.h"

#include "mcc/mcc.h"
#include "mcc/material.h"
#include "mcc/font/font.h"
#include "mcc/window.h"
#include "mcc/settings.h"
#include "mcc/camera/perspective_camera.h"
#include "mcc/engine/engine.h"
#include "mcc/renderer/renderer.h"
#include "mcc/physics/physics.h"
#include "mcc/terrain/terrain.h"
#include "mcc/shader/shader.h"

#include "mcc/resource.h"

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
  texture::Texture::Init();

  settings::Init();
  font::Initialize();
  Entities::Initialize();
  const auto loop = uv_loop_new();
  Engine::Init(loop);
  gui::Screen::Init();
  renderer::Renderer::Init();
  camera::PerspectiveCameraBehavior::Init();
  physics::PhysicsSimulator::Init();
  mesh::Mesh::Init();
  Window::Init();
  terrain::Terrain::Init();
  resource::Registry::Init();
  d2::Mesh::Init();
  FrameBuffer::Init();

  Engine::Run();
  return EXIT_SUCCESS;
}