#ifndef MCC_WINDOW_BUILDER_H
#define MCC_WINDOW_BUILDER_H

#include <string>
#include "mcc/rx.h"
#include "mcc/builder.h"
#include "mcc/window/window_constants.h"

namespace mcc::window {
  class Window;
  class WindowBuilder : public BuilderTemplate<Window> {
  private:
    bool fullscreen_;
    bool resizable_;
    std::string title_;
    glm::u32vec2 size_;
  public:
    WindowBuilder() = default;
    virtual ~WindowBuilder() = default;

    void SetFullscreen(const bool value = true) {
      fullscreen_ = value;
    }

    inline void ClearFullscreen() {
      return SetFullscreen(false);
    }

    bool IsFullscreen() const {
      return fullscreen_;
    }

    void SetResizable(const bool value = true) {
      resizable_ = value;
    }

    inline void ClearResizable() {
      return SetResizable(false);
    }

    bool IsResizable() const {
      return resizable_;
    }

    void SetTitle(const std::string& value) {
      title_ = value;
    }

    const std::string& GetTitle() const {
      return title_;
    }

    void SetSize(const glm::u32vec2& size) {
      size_ = size;
    }

    const glm::u32vec2& GetSize() const {
      return size_;
    }

    Window* Build() const override;
    rx::observable<Window*> BuildAsync() const override;
  };
}

#endif //MCC_WINDOW_BUILDER_H