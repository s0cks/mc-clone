#include <cstdlib>
#include <glog/logging.h>

#include <time.h>
#include <cstdlib>
#include <uv.h>

#include <units.h>
#include <backward.hpp>

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

#include "mcc/signals.h"
#include "mcc/mouse/cursor.h"

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
  mcc::mouse::Cursor* grab_cursor_;

  void OnMouseEnter(mcc::gui::MouseEnterEvent* event) override {
    DLOG(INFO) << "enter.";
    SetBackground(mcc::Color(255, 0, 0, 255 / 2));
    mcc::mouse::SetCurrentCursor(grab_cursor_);
  }

  void OnMouseExit(mcc::gui::MouseExitEvent* event) override {
    DLOG(INFO) << "exit.";
    SetBackground(mcc::kBlack);
    mcc::mouse::SetCurrentCursor(nullptr);
  }
  
  void OnMouseClick(mcc::gui::MouseClickEvent* event) override {
    DLOG(INFO) << "clicked.";
  }
public:
  TestWindow():
    Window(),
    OnMouseEnterEvent(this),
    OnMouseExitEvent(this),
    OnMouseClickEvent(this),
    grab_cursor_(mcc::mouse::GlfwCursor::New("bibata-modern-ice/hand1.png")) {
    SetPos({ 0, 0 });
    SetSize({ 128, 128 });
    SetBackground(mcc::kBlack);
  }
  ~TestWindow() override = default;
};

static inline void
OnUnhandledException() {
  using namespace backward;
  StackTrace st;
  st.load_here(32);
  Printer p;
  p.print(st);
}

int main(int argc, char** argv) {  
  using namespace mcc;
  srand(time(NULL));
  ::google::InitGoogleLogging(argv[0]);
  ::google::LogToStderr();
  ::google::ParseCommandLineFlags(&argc, &argv, true);
  mcc::InitSignalHandlers();
  std::set_terminate(OnUnhandledException);
  LOG_IF(FATAL, !mcc::SetCurrentThreadName("main")) << "failed to set main thread name.";
  gfx::Init();
  font::Initialize();
  Window::Init();
  mouse::InitMouse();
  keyboard::InitKeyboard();
  render::Renderer::Init();

  window::OnWindowOpenedEvent()
    .subscribe([](WindowOpenedEvent* event) {
      const auto window = event->window();
      mouse::SetCurrentCursor("cursor://bibata-modern-ice/left_ptr.png");
    });

  const auto engine = engine::Engine::GetEngine();
  const auto keyboard = GetKeyboard();
  keyboard->OnPressed(GLFW_KEY_SPACE)
    .subscribe([engine](keyboard::KeyPressedEvent* event) {
      new TestWindow();
    });
  keyboard->OnPressed(GLFW_KEY_ESCAPE)
    .subscribe([engine](keyboard::KeyPressedEvent* event) {
      engine->Shutdown();
    });
  engine->Run();
  return EXIT_SUCCESS;
}