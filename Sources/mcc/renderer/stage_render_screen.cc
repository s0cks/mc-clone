#include "mcc/renderer/stage.h"
#include "mcc/renderer/renderer.h"
#include "mcc/window.h"
#include "mcc/gui/gui.h"

namespace mcc::renderer {
  void RenderScreenStage::Render(const Tick& tick, const glm::mat4& projection, const glm::mat4& view) {
    DLOG(INFO) << "drawing screen....";
    const auto size = Window::GetSize();
    auto proj = glm::mat4(1.0f);
    proj = glm::ortho(0.0f, size[0], size[1], 0.0f);
    gui::Screen::RenderScreen(proj);
  }
}