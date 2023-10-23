#ifndef MCC_WINDOW_H
#define MCC_WINDOW_H

#include <uv.h>
#include <glm/glm.hpp>
#include <glog/logging.h>

#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/render_loop.h"
#include "mcc/shape/square.h"
#include "mcc/scene.h"
#include "mcc/renderer/renderer.h"

namespace mcc {
  class Window : public scene::WindowNode {
  public:
    static constexpr const glm::vec4 kDefaultBackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  protected:
    GLFWwindow* handle_;
    glm::dvec2 init_size_;
    glm::dvec2 size_;
    glm::vec4 bg_color_;

    Window(const glm::dvec2 init_size):
      scene::WindowNode(),
      handle_(nullptr),
      size_(init_size),
      init_size_(init_size),
      bg_color_(kDefaultBackgroundColor) {
    }

    void Render();

    static void OnPreRender();
    static void OnRender();
    static void OnPostRender();
    static void SetWindow(Window* window);
    static GLFWwindow* CreateGlfwWindow(const int width, const int height);

    static void OnEscapePressed();
  public:
    virtual ~Window() = default;
    DEFINE_NON_COPYABLE_TYPE(Window);

    inline GLFWwindow* handle() const {
      return handle_;
    }

    glm::dvec2 GetInitSize() const {
      return init_size_;
    }

    glm::dvec2 GetSize() const {
      return size_;
    }

    glm::vec4 GetBackgroundColor() const {
      return bg_color_;
    }

    void Open();
  public:
    static Window* Initialize(const glm::dvec2 init_size);
    static Window* GetWindow();
  };
}

#endif //MCC_WINDOW_H