#include <cstdlib>
#include <glog/logging.h>

#include <time.h>
#include <cstdlib>
#include <uv.h>

#include <units.h>

#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION

#include "mcc/mcc.h"
#include "mcc/ibo/ibo.h"
#include "mcc/font/font.h"
#include "mcc/os_thread.h"
#include "mcc/mouse/mouse.h"
#include "mcc/entity/entity.h"
#include "mcc/window/window.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"
#include "mcc/render/renderer.h"
#include "mcc/resource/resource.h"
#include "mcc/keyboard/keyboard.h"
#include "mcc/material/material.h"

#include "mcc/gfx_debug.h"

#include "mcc/gui/gui_window.h"
#include "mcc/gui/gui_on_mouseenter.h"
#include "mcc/gui/gui_on_mouseexit.h"
#include "mcc/gui/gui_on_mouseclick.h"

#include "mcc/fbo/fbo.h"
#include "mcc/fbo/fbo_factory.h"
#include "mcc/render/render_settings.h"

#include "mcc/skybox/skybox.h"

#include "mcc/mouse/mouse_flags.h"

template<class Event, const google::LogSeverity Severity = google::INFO>
static inline std::function<void(Event*)>
LogEvent() {
  return [](Event* event) {
    LOG_AT_LEVEL(Severity) << event->ToString();
  };
}

class TestWindow : public mcc::gui::Window,
                   public mcc::gui::OnMouseEnterEvent,
                   public mcc::gui::OnMouseExitEvent,
                   public mcc::gui::OnMouseClickEvent {
protected:
  void OnMouseEnter(mcc::gui::MouseEnterEvent* event) override {
    DLOG(INFO) << "enter.";
    SetBackground(mcc::Color(255, 0, 0, 255 / 2));
  }

  void OnMouseExit(mcc::gui::MouseExitEvent* event) override {
    DLOG(INFO) << "exit.";
    SetBackground(mcc::kBlack);
  }
  
  void OnMouseClick(mcc::gui::MouseClickEvent* event) override {
    DLOG(INFO) << "clicked.";
  }
public:
  TestWindow():
    Window(),
    OnMouseEnterEvent(this),
    OnMouseExitEvent(this),
    OnMouseClickEvent(this) {
    SetPos({ 0, 0 });
    SetSize({ 128, 128 });
    SetBackground(mcc::kBlack);
  }
  ~TestWindow() override = default;
};

int main(int argc, char** argv) {
  using namespace mcc;
  srand(time(NULL));
  ::google::InitGoogleLogging(argv[0]);
  ::google::LogToStderr();
  ::google::ParseCommandLineFlags(&argc, &argv, true);
  LOG_IF(FATAL, !mcc::SetCurrentThreadName("main")) << "failed to set main thread name.";
  glfwSetErrorCallback(&mcc::OnGlfwError);
  LOG_IF(FATAL, !glfwInit()) << "error initializing GLFW";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MCC_GLFW_CTX_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MCC_GLFW_CTX_VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHintString(GLFW_COCOA_FRAME_NAME, "Hello");
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_TRUE);
#endif
  gfx::EnableDebug();
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

  font::Initialize();
  Window::Init();
  mouse::InitMouse();
  keyboard::InitKeyboard();
  render::Renderer::Init();

  mouse::OnMouseEvent()
    .subscribe(LogEvent<mouse::MouseEvent>());

  material::OnMaterialCreatedEvent()
    .subscribe(LogEvent<material::MaterialCreatedEvent>());
    
  const auto skybox = Skybox::New("texture://space_nebulas");
  const auto engine = engine::Engine::GetEngine();
  const auto keyboard = GetKeyboard();
  keyboard->OnPressed(GLFW_KEY_ESCAPE)
    .subscribe([engine](keyboard::KeyPressedEvent* event) {
      engine->Shutdown();
    });
  keyboard->OnPressed(GLFW_KEY_SPACE)
    .subscribe([skybox](keyboard::KeyPressedEvent* event) {
      skybox::SetCurrentSkybox(skybox);
    });
  engine->Run();
  return EXIT_SUCCESS;
}