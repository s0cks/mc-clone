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

#include "mcc/input/mouse.h"
#include "mcc/input/keyboard.h"

#include "mcc/engine/engine.h"
#include "mcc/renderer/renderer.h"
#include "mcc/camera/perspective_camera.h"

#include "mcc/thread_local.h"
#include "mcc/gui/gui_frame.h"

#include "mcc/physics/force.h"
#include "mcc/physics/transform.h"
#include "mcc/physics/rigid_body.h"

#include "mcc/light/point.h"
#include "mcc/light/directional.h"

#include "mcc/shader/shader.h"

#include "mcc/resource.h"

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
    //glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    SetHandle(handle);
  }

  void Window::OnInit() {
    
  }

  void Window::OnPostInit() {
    const auto e3 = Entities::CreateEntity();
    e3.AddComponent<renderer::Renderable>({
      .shader = GetShader("light_sphere"),
      .mesh = mesh::LoadFrom("/sphere/sphere.obj"),
    });
    e3.AddComponent<physics::Transform>({
      .position = glm::vec3(0.0f, 3.0f, 0.0f),
      .rotation = glm::vec3(0.0f),
      .scale = glm::vec3(1.0f),
    });
    e3.AddComponent<light::PointLight>({
      {
        .ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f),
        .diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
        .specular = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
      },
      .position = glm::vec4(0.0f, 3.0f, 0.0f, 1.0f),
      .constant = 1.0f,
      .linear = 0.09f,
      .quadratic = 0.032f,
    });
    DLOG(INFO) << "light sphere: " << e3;

    const auto e4 = Entities::CreateEntity();
    e4.AddComponent<light::DirectionalLight>(light::DirectionalLight{
      { 
        .ambient = glm::vec4(0.05f, 0.05f, 0.5f, 1.0f),
        .diffuse = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f),
        .specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
      },
      .direction = glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f),
    });

    // const auto texture = texture::Texture::LoadFrom(FLAGS_resources + "/textures/container.png");
    // const auto e2 = Entities::CreateEntity();
    // const auto mesh = mesh::NewCube();
    // Coordinator::AddComponent(e2, renderer::Renderable {
    //   .shader = shader::Shader::Get("cube"),
    //   .mesh = mesh,
    //   .texture = texture,
    //   .material = GetMaterial("emerald"),
    // });
    // Coordinator::AddComponent(e2, physics::Transform {
    //   .position = glm::vec3(0.0f, 0.0f, 0.0f),
    //   .rotation = glm::vec3(0.0f),
    //   .scale = glm::vec3(1.0f),
    // });
    // Coordinator::AddComponent(e2, physics::RigidBody {
    //   .mass = 1.0f,
    //   .velocity = glm::vec3(0.0f),
    // });
    // DLOG(INFO) << "cube: " << e2;

    Mouse::GetButtonEventSubject()
      .get_observable()
      .filter([](const MouseButtonEvent& e) {
        return e.button == MouseButton::kButton1
            && e.state == kPressed;
      })
      .subscribe([](const MouseButtonEvent& e) {
        const auto entity = Mouse::CastRayTo(1.0f);
        if(entity) {
          DLOG(INFO) << "found entity: " << (*entity);
        } else {
          DLOG(INFO) << "couldn't find an entity.";
        }
      });
  }

  void Window::OnTerminating() {
    glfwSetWindowShouldClose(GetHandle(), GLFW_TRUE);
  }

  void Window::OnTerminated() {
    glfwTerminate();
  }
}