#include "mcc/gui/gui_window.h"
#include "mcc/gui/gui_vertex.h"
#include "mcc/vao.h"
#include "mcc/vbo.h"

#include "mcc/font/font.h"
#include "mcc/font/font_renderer.h"
#include "mcc/shape/square.h"

namespace mcc::gui {
  static Window::WindowPtr current_;
  static Vao vao_(kInvalidVao);
  static Shader shader_;
  static font::Font* font_ = nullptr;

  Window::WindowPtr Window::GetCurrent() {
    return current_;
  }

  void Window::SetCurrent(Window::WindowPtr value) {
    current_ = value;
  }

  bool Window::HasCurrent() {
    return current_ != nullptr;
  }

  void Window::Init() {
    vao_ = Vao();
    shader_ = CompileShader("gui");
    font_ = new font::Font("arial/arial");
  }

  Window::Window(const glm::vec2 size):
    ContainerComponent(),
    title_("Hello World"),
    size_(size),
    color_(glm::vec3(0.5f, 0.5f, 0.5f)),
    pos_(glm::vec2(0.0f, 0.0f)),
    vbo_(),
    vertices_() {
    vertices_.push_back(Vertex {
      .pos = glm::vec2(pos_[0]          , pos_[1]), // top left
      .color = color_,
    });
    vertices_.push_back(Vertex {
      .pos = glm::vec2(pos_[0] + size[0], pos_[1]), // top right
      .color = color_,
    });
    vertices_.push_back(Vertex {
      .pos = glm::vec2(pos_[0]          , pos_[1] + size[1]), // bottom left
      .color = color_,
    });
    vertices_.push_back(Vertex {
      .pos = glm::vec2(pos_[0] + size[0], pos_[1]), // top right
      .color = color_,
    });
    vertices_.push_back(Vertex {
      .pos = glm::vec2(pos_[0] + size[0], pos_[1] + size[1]), // bottom right
      .color = color_,
    });
    vertices_.push_back(Vertex {
      .pos = glm::vec2(pos_[0]          , pos_[1] + size[1]), // bottom left
      .color = color_,
    });

    VaoScope vao(vao_);
    vbo_ = GuiVertexBufferObject(vertices_);
  }

  void Window::Render(const glm::mat4 proj, const glm::mat4 view) {
    glm::mat4 model(1.0f);
    shader_.ApplyShader();
    shader_.SetMat4("projection", proj);
    shader_.SetMat4("model", model);
    shader_.SetMat4("view", view);
    shader_.ApplyShader();
    {
      VaoScope vao(vao_);
      glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
    }

    font::FontRenderer font(font_);
    font.RenderText(title_, pos_ + glm::vec2(32.0f, 16.0f));
  }
}