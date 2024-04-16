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
#include "mcc/material/material.h"
#include "mcc/font/font.h"
#include "mcc/window/window.h"
#include "mcc/camera/perspective_camera.h"
#include "mcc/engine/engine.h"
#include "mcc/renderer/renderer.h"
#include "mcc/physics/physics.h"
#include "mcc/terrain/terrain.h"
#include "mcc/shader/shader.h"

#include "mcc/resource/resource.h"

#include "mcc/os_thread.h"

int main(int argc, char** argv) {
  srand(time(NULL));
  ::google::InitGoogleLogging(argv[0]);
  ::google::LogToStderr();
  ::google::ParseCommandLineFlags(&argc, &argv, true);

  LOG_IF(FATAL, !mcc::SetCurrentThreadName("main")) << "failed to set main thread name.";
  glfwSetErrorCallback(&mcc::OnGlfwError);
  LOG_IF(FATAL, !glfwInit()) << "error initializing GLFW";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
  glfwWindowHintString(GLFW_COCOA_FRAME_NAME, "Hello");
  glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_TRUE);
#endif
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
  glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
  glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);
  glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
  glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

  using namespace mcc;
  res::OnResourceEvent().subscribe([](res::ResourceEvent* event) {
    DLOG(INFO) << "resource event: " << event->ToString();
  });

  //TODO: settings::Init();
  font::Initialize();
  Entities::Initialize();
  gui::Screen::Init();
  Window::Init();
  mouse::InitMouse();
  renderer::Renderer::Init();
  physics::PhysicsSimulator::Init();
  mesh::Mesh::Init();
  terrain::Terrain::Init();
  d2::Mesh::Init();
  FrameBuffer::Init();

  const auto engine = engine::Engine::GetEngine();
  engine->Run();
  return EXIT_SUCCESS;
}