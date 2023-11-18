#ifndef MCC_GUI_FRAME_H
#define MCC_GUI_FRAME_H

#include <string>
#include <memory>

#include "mcc/gfx.h"
#include "mcc/gui/gui.h"

namespace mcc::renderer {
  class Renderer;
}

namespace mcc::gui {
  class FrameRenderer;
  class Frame;
  typedef std::shared_ptr<Frame> FramePtr;

  class FrameVisitor {
  protected:
    FrameVisitor() = default;
  public:
    virtual ~FrameVisitor() = default;
    virtual bool Visit(FramePtr frame) = 0;
  };

  class Frame {
    friend class FrameRenderer;
  public:
    enum Position {
      kTopLeft,
      kTop,
      kTopRight,

      kLeft,
      kRight,
      kCenter,

      kBottomLeft,
      kBottom,
      kBottomRight,
    };
  protected:
    std::string name_;
    glm::vec2 pos_;
    glm::vec2 size_;
    int flags_;

    Frame(const std::string& name,
          const glm::vec2 pos,
          const glm::vec2 size,
          const int flags = 0):
      name_(name),
      pos_(pos),
      size_(size),
      flags_(flags) {
    }
    Frame(const std::string& name, const Position pos, const glm::vec2 size, const int flags = NK_WINDOW_TITLE);

    virtual void Render(nk::Context* ctx) {

    }

    virtual void PreRender(nk::Context* ctx);
    virtual void PostRender(nk::Context* ctx);
  private:
    void RenderFrame(nk::Context* ctx) {
      PreRender(ctx);
      Render(ctx);
      PostRender(ctx);
    }
  public:
    virtual ~Frame() = default;

    friend std::ostream& operator<<(std::ostream& stream, const Frame& rhs) {
      stream << "gui::Frame(";
      stream << "name=" << rhs.name_ << ", ";
      stream << "pos=" << glm::to_string(rhs.pos_) << ", ";
      stream << "size=" << glm::to_string(rhs.size_);
      stream << ")";
      return stream;
    }
  };

  class FrameRenderer : public gui::FrameVisitor {
  private:
    nk::Context* ctx_;
  public:
    explicit FrameRenderer(nk::Context* ctx):
      FrameVisitor(),
      ctx_(ctx) {
    }
    ~FrameRenderer() override = default;

    bool Visit(gui::FramePtr frame) override {
      frame->RenderFrame(ctx_);
      return true;
    }
  };
}

#endif //MCC_GUI_FRAME_H