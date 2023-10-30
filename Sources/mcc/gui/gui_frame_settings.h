#ifndef MCC_GUI_FRAME_SETTINGS_H
#define MCC_GUI_FRAME_SETTINGS_H

#include "mcc/gui/gui_frame.h"

namespace mcc::gui {
  class SettingsFrame : public Frame {
  protected:
    void Render(nk::Context* ctx) override;
  public:
    SettingsFrame();
    ~SettingsFrame() override = default;
  public:
    static inline FramePtr New() {
      return std::make_shared<SettingsFrame>();
    }
  };
}

#endif //MCC_GUI_FRAME_SETTINGS_H