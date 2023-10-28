#ifndef MCC_GUI_WINDOW_H
#define MCC_GUI_WINDOW_H

#include "mcc/gui/gui_component.h"
#include "mcc/shape/square.h"

namespace mcc::gui {
  class Window : public ContainerComponent {
  private:
    std::string title_;
    Shader shader_;
    glm::vec2 dimension_;
    float scale_;
    float aspect_;
    float near_;
    float far_;
    mesh::Mesh* mesh_;
  public:
    Window(const float width, 
           const float height,
           const float scale = 1.0f,
           const float near = 0.0f,
           const float far = 10.0f):
      ContainerComponent(),
      title_(),
      shader_(CompileShader("gui_window")),
      dimension_(width, height),
      scale_(scale),
      aspect_(width / height),
      near_(near),
      far_(far),
      mesh_(nullptr) {
    }
    ~Window() override = default;

    void Render() override {
      
    }
    
    void Render(const glm::mat4 proj, const glm::mat4 view);

    void SetTitle(const std::string& value) {
      title_ = value;
    }
  public:
    typedef std::shared_ptr<Window> WindowPtr;

    static WindowPtr New(const float width, const float height) {
      return std::make_shared<Window>(width, height);
    }

    static WindowPtr GetCurrent();
    static void SetCurrent(WindowPtr value);
    static bool HasCurrent();
   
    template<class W>
    static inline void
    SetCurrent() {
      return SetCurrent(W::New());
    }
  };
}

#endif //MCC_GUI_WINDOW_H