#ifndef MCC_GUI_WINDOW_H
#define MCC_GUI_WINDOW_H

#include "mcc/gui/gui_component.h"
#include "mcc/gui/gui_vertex.h"

namespace mcc::gui {
  class Window : public ContainerComponent {
  private:
    std::string title_;
    glm::vec2 pos_;
    glm::vec2 size_;
    glm::vec3 color_;
    GuiVertexBufferObject vbo_;
    VertexList vertices_;
  public:
    Window(const glm::vec2 size = glm::vec2(400.0f, 400.0f));
    ~Window() override = default;

    void Render() override {
      
    }
    
    void Render(const glm::mat4 proj, const glm::mat4 view);

    void SetTitle(const std::string& value) {
      title_ = value;
    }
  public:
    static void Init();

    typedef std::shared_ptr<Window> WindowPtr;

    static WindowPtr New(const glm::vec2 size) {
      return std::make_shared<Window>(size);
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