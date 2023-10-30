#include <glog/logging.h>
#include <uv.h>
#include <cstdlib>
#include <cstdio>

#include "mcc/gui/gui.h"

#include "mcc/flags.h"
#include "mcc/window.h"
#include "mcc/shader/shader.h"
#include "mcc/camera/camera.h"
#include "mcc/thread_local.h"
#include "mcc/uv_utils.h"

#include "mcc/font/font.h"
#include "mcc/font/font_renderer.h"
#include "mcc/mesh/mesh.h"
#include "mcc/shape/square.h"
#include "mcc/shape/cube.h"

#include "mcc/mouse/mouse.h"
#include "mcc/keyboard/keyboard.h"

#include "mcc/ecs/coordinator.h"
#include "mcc/engine/engine.h"
#include "mcc/renderer/renderer.h"
#include "mcc/camera/perspective_camera.h"

#include "mcc/thread_local.h"
#include "mcc/gui/gui_frame.h"

namespace mcc {
  typedef struct nk_context NuklearContext;

  static ThreadLocal<GLFWwindow> handle_;
  static ThreadLocal<NuklearContext> nk_ctx_;

  static std::vector<gui::FramePtr> frames_;

  void Window::AddFrame(gui::FramePtr frame) {
    frames_.push_back(frame);
  }

  bool Window::VisitFrames(gui::FrameVisitor* vis) {
    for(auto& frame : frames_) {
      if(!vis->Visit(frame))
        return false;
    }
    return true;
  }

  static glm::vec2 size_;
  static std::string title_;

  void Window::Init() {
    if(!Window::IsFullscreen()) {
      SetSize(Window::GetInitialSize());
    }

    auto ctx = new NuklearContext();
    nk_init_default(ctx, NULL);
    nk_ctx_.Set(ctx);

    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Engine::OnTerminating(&OnTerminating);
    Engine::OnTerminated(&OnTerminated);
  }

  void Window::SetHandle(GLFWwindow* handle) {
    handle_.Set(handle);
  }

  GLFWwindow* Window::GetHandle() {
    return handle_.Get();
  }

  void Window::SetSize(const glm::vec2 size) {
    size_ = size;
  }

  glm::vec2 Window::GetSize() {
    return size_;
  }

  float Window::GetWidth() {
    return size_[0];
  }

  float Window::GetHeight() {
    return size_[1];
  }

  static inline void
  OnWindowResized(GLFWwindow* window, int width, int height) {
    const auto orthoCamera = OrthoCamera::Get();
    orthoCamera->SetSize(glm::vec2(width, height));
    const auto perspectiveCamera = PerspectiveCamera::Get();
    perspectiveCamera->SetSize(glm::vec2(width, height));
  }

  void Window::Close() {
    glfwSetWindowShouldClose(GetHandle(), GL_TRUE);
  }

  void Window::OnWindowClosed(GLFWwindow* window) {
    Engine::Shutdown();
  }

  void Window::OnPreInit() {
    const auto handle = glfwCreateWindow(GetWidth(), GetHeight(), "Hello World", NULL, NULL);
    if(!handle) {
      glfwTerminate();
      LOG(FATAL) << "failed to create GLFW window";
    }
    glfwMakeContextCurrent(handle);
    glfwSwapInterval(0);
    glfwSetWindowCloseCallback(handle, &OnWindowClosed);
    //TODO: glfwSetFramebufferSizeCallback(handle_, &OnWindowResized);

    SetHandle(handle);
  }

  void Window::OnInit() {
    
  }

  void Window::OnPostInit() {
    glViewport(0, 0, size_[0], size_[1]);

    const auto texture = texture::Texture::LoadFrom(FLAGS_resources + "/textures/container.png");
    const auto shader = CompileShader("cube");
    const auto e2 = Entities::CreateEntity();
    const auto mesh = Cube::CreateMesh();
    Coordinator::AddComponent(e2, renderer::Renderable {
      .shader = shader,
      .mesh = mesh,
      .texture = texture,
    });
  }

  void Window::OnTerminating() {
    glfwSetWindowShouldClose(GetHandle(), GLFW_TRUE);
  }

  void Window::OnTerminated() {
    glfwTerminate();
  }
}