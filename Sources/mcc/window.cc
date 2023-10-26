#include <glog/logging.h>
#include <uv.h>
#include <cstdlib>
#include <cstdio>

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
#include "mcc/shape/cube.h"

#include "mcc/mouse/mouse.h"
#include "mcc/keyboard/keyboard.h"

#include "mcc/ecs/coordinator.h"
#include "mcc/engine/engine.h"
#include "mcc/renderer/renderer.h"
#include "mcc/camera/perspective_camera.h"

namespace mcc {
  GLFWwindow* handle_;

  static inline void
  OnGlfwError(int code, const char* description) {
    LOG(ERROR) << "GLFW error: [" << code << "]" << ": " << description;
  }

  static inline void
  OnWindowResized(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    const auto orthoCamera = OrthoCamera::Get();
    orthoCamera->SetSize(glm::vec2(width, height));
    const auto perspectiveCamera = PerspectiveCamera::Get();
    perspectiveCamera->SetSize(glm::vec2(width, height));
  }

  void Window::Close() {
    glfwSetWindowShouldClose(handle_, GL_TRUE);
  }

  void Window::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
  }

  void Window::OnPreInit() {
    glfwSetErrorCallback(&OnGlfwError);
    LOG_IF(FATAL, !glfwInit()) << "error initializing GLFW";
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  }

  void Window::OnInit() {
    const auto size = Window::GetSize();
    const auto handle = glfwCreateWindow(size[0], size[1], "Hello World", NULL, NULL);
    if(!handle) {
      glfwTerminate();
      LOG(FATAL) << "failed to create GLFW window";
    }
    handle_ = handle;
  }

  void Window::OnPostInit() {
    glfwMakeContextCurrent(handle_);
    glfwSwapInterval(0);
    //TODO: glfwSetFramebufferSizeCallback(handle_, &OnWindowResized);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const auto texture = texture::Texture::LoadFrom(FLAGS_resources + "/textures/container.png");

    ProgramCompiler compiler;
    compiler.AttachFragmentShaderFromFile(FLAGS_resources + "/shaders/cube.fsh");
    compiler.AttachVertexShaderFromFile(FLAGS_resources + "/shaders/cube.vsh");
    Shader shader;
    LOG_IF(FATAL, !compiler.Compile(&shader)) << "failed to compile cube shader.";

    const auto e2 = Entities::CreateEntity();
    const auto mesh = Cube::CreateMesh();
    Coordinator::AddComponent(e2, Renderable {
      .shader = shader,
      .mesh = mesh,
      .texture = texture,
    });
  }

  GLFWwindow* Window::GetHandle() {
    return handle_;
  }

  glm::vec2 Window::GetSize() {
    return glm::vec2(800, 800);
  }
  
  glm::vec2 Window::GetInitSize() {
    return glm::vec2(800, 800);
  }
}