#ifndef MCC_WINDOW_BUILDER_H
#define MCC_WINDOW_BUILDER_H

namespace mcc::window {
  class Window;
  class WindowBuilder {
  public:
    WindowBuilder() = default;
    virtual ~WindowBuilder() = default;
  };
}

#endif //MCC_WINDOW_BUILDER_H