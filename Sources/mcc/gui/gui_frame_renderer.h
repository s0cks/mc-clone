#ifndef MCC_GUI_FRAME_RENDERER_H
#define MCC_GUI_FRAME_RENDERER_H

#include "mcc/gui/gui_frame.h"

namespace mcc::gui {
  class RendererFrame : public Frame {
  protected:
    void Render(nk::Context* ctx) override;
  public:
    RendererFrame();
    ~RendererFrame() override = default;
  public:
    static inline FramePtr New() {
      return std::make_shared<RendererFrame>();
    }
  };
}

#endif //MCC_GUI_FRAME_RENDERER_H