#include <glog/logging.h>
#include <uv.h>

#include <OpenGL/gl3.h>

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

namespace mcc {
  static ThreadLocal<Window> window_;

  Window* Window::Initialize(const glm::dvec2 init_size) {
    //TODO: check for open window
    const auto instance = new Window(init_size);
    SetWindow(instance);
    return instance;
  }

  Window* Window::GetWindow() {
    return window_.Get();
  }

  void Window::SetWindow(Window* window) {
    window_.Set(window);
  }

  void Window::OnEscapePressed() {
    DLOG(INFO) << "escape pressed.";
    const auto window = Window::GetWindow();
    glfwSetWindowShouldClose(window->handle(), GL_TRUE);
  }

  static inline void
  OnGlfwError(int code, const char* description) {
    LOG(ERROR) << "GLFW error: [" << code << "]" << ": " << description;
  }

  static inline void
  OnWindowResized(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    const auto orthoCamera = OrthoCamera::Get();
    orthoCamera->UpdateSize(glm::vec2(width, height));
  }

  GLFWwindow* Window::CreateGlfwWindow(const int width, const int height) {
    glfwSetErrorCallback(&OnGlfwError);
    LOG_IF(FATAL, !glfwInit()) << "error initializing GLFW";
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* handle = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if(!handle) {
      glfwTerminate();
      LOG(FATAL) << "failed to create GLFW window";
    }

    glfwMakeContextCurrent(handle);
    glfwSetFramebufferSizeCallback(handle, &OnWindowResized);

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    return handle;
  }

  void Window::OnPreRender() {
    DLOG(INFO) << "pre-render (1).";
    const auto window = Window::GetWindow();
    const auto bg_color = window->GetBackgroundColor();
    glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  }

  void Window::OnRender() {
    const auto window = Window::GetWindow();
    scene::NodeRenderer::Render(window);
  }

  void Window::OnPostRender() {
    const auto window = Window::GetWindow();
    glfwSwapBuffers(window->handle_);
    glfwPollEvents();
  }
  
  void Window::Open() {
    handle_ = CreateGlfwWindow(size_[0], size_[1]);
    const auto orthoCamera = OrthoCamera::Initialize({ static_cast<float>(size_[0]), static_cast<float>(size_[1]) });
    const auto loop = RenderLoop::GetRenderLoop();
    const auto escapeListener = new KeyPressedListener(KeyCode::kKeyEscape, &OnEscapePressed);
    PreRenderHandle preRender(loop->loop(), &OnPreRender);
    RenderHandle render(loop->loop(), &OnRender);
    PostRenderHandle postRender(loop->loop(), &OnPostRender);
    
    const auto font = new font::Font("arial/arial");

    Square square(glm::vec2(25.0f, 25.0f), 50.0f);
    AppendChild(&square);

    while(!glfwWindowShouldClose(handle_)) {
      loop->Run(RenderLoop::kNoWait);
    }

    glfwTerminate();
  }
}