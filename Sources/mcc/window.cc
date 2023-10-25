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

#include "mcc/camera/perspective_camera.h"

namespace mcc {
  static ThreadLocal<Window> window_;

  Window* Window::Initialize(glm::dvec2 init_size) {
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
    orthoCamera->SetSize(glm::vec2(width, height));
    const auto perspectiveCamera = PerspectiveCamera::Get();
    perspectiveCamera->SetSize(glm::vec2(width, height));
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
    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(handle, &OnWindowResized);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return handle;
  }

  void Window::OnPreRender() {
    const auto window = Window::GetWindow();
    const auto bg_color = window->GetBackgroundColor();
    glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  }

  static constexpr const font::FontSize kFontSize = 24;
  static font::Font* font_;

  void Window::OnRender() {
    const auto loop = RenderLoop::GetRenderLoop();
    const auto window = Window::GetWindow();

    const auto topLeft = glm::vec2(0, window->GetSize()[1]);

    font::FontRenderer font(font_);
    
    static constexpr const uint64_t kCounterLength = 3 + 2 + 4;
    
    char fps[kCounterLength];
    snprintf(fps, kCounterLength, "FPS: %" PRIu64, Renderer::GetFPS());
    font.RenderText(fps, glm::vec2(topLeft[0] + 5.0f, topLeft[1] - kFontSize));
    char tps[kCounterLength];
    snprintf(tps, kCounterLength, "TPS: %" PRIu64, Engine::GetTPS());
    font.RenderText(tps, glm::vec2(topLeft[0] + 5.0f, topLeft[1] - (kFontSize * 2)));

    std::stringstream ss;
    ss << "Mouse POS: " << Mouse::GetPosition() << "; Delta: " << Mouse::GetDelta();
    font.RenderText(ss.str(), glm::vec2(topLeft[0] + 5.0f, topLeft[1] - (kFontSize * 3)));
    
    loop->frames_ += 1;
  }

  void Window::OnPostRender() {
    const auto window = Window::GetWindow();
    glfwSwapBuffers(window->handle_);
    glfwPollEvents();
  }
  
  void Window::Open() {
    handle_ = CreateGlfwWindow(size_[0], size_[1]);
    Mouse::Initialize(handle_);
    
    camera::PerspectiveCameraBehavior::Init();
    Renderer::Init();

    const auto windowSize = glm::vec2(static_cast<float>(size_[0]), static_cast<float>(size_[1]));
    const auto orthoCamera = OrthoCamera::Initialize(windowSize);
    const auto loop = RenderLoop::GetRenderLoop();

    PreRenderHandle preRender(loop, &OnPreRender);
    RenderHandle render(loop, &OnRender);
    PostRenderHandle postRender(loop, &OnPostRender);
    
    font_ = new font::Font("arial/arial", kFontSize);
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

    while(!glfwWindowShouldClose(handle_)) {
      loop->Run(uv::Loop::kRunNoWait);
    }

    glfwTerminate();
  }
}