#include "mcc/window/window_builder.h"

namespace mcc::window {
  Window* WindowBuilder::Build() const {
    NOT_IMPLEMENTED(FATAL);
    return nullptr;
  }

  rx::observable<Window*> WindowBuilder::BuildAsync() const {
    return rx::observable<>::create<Window*>([this](rx::subscriber<Window*> s) {
      const auto value = Build();
      if(!value) {
        const auto err = "failed to build Window.";
        return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
      }

      s.on_next(value);
      s.on_completed();
    });
  }
}