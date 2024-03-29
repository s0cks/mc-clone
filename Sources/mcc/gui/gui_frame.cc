#include "mcc/gui/gui_frame.h"

#include "mcc/window/window.h"

namespace mcc::gui {
#define WINDOW_WIDTH (win_size[0])
#define WINDOW_HEIGHT (win_size[1])

#define X (pos[0])
#define Y (pos[1])
#define WIDTH (size[0])
#define HEIGHT (size[1])

#define FRAME_X (pos_[0])
#define FRAME_Y (pos_[1])
#define FRAME_WIDTH (size_[0])
#define FRAME_HEIGHT (size_[1])

  Frame::Frame(const std::string& name, const Frame::Position pos, const glm::vec2 size, const int flags):
    name_(name),
    pos_(0.0f, 0.0f),
    size_(size),
    flags_(flags) {
    const auto win_size = Window::Get()->GetSize();
    switch(pos) {
      case kTop:
        FRAME_WIDTH = WINDOW_WIDTH;
        break;
      case kBottom:
        FRAME_WIDTH = WINDOW_WIDTH;
        FRAME_Y = WINDOW_HEIGHT - FRAME_HEIGHT;
        break;
      case kLeft:
        FRAME_HEIGHT = WINDOW_HEIGHT;
        break;
      case kRight:
        FRAME_HEIGHT = WINDOW_HEIGHT;
        FRAME_X = WINDOW_WIDTH - FRAME_WIDTH;
        break;
      case kTopRight:
        FRAME_X = WINDOW_WIDTH - FRAME_WIDTH;
        break;
      case kBottomRight:
        FRAME_X = WINDOW_WIDTH - FRAME_WIDTH;
        FRAME_Y = WINDOW_HEIGHT - FRAME_HEIGHT;
        break;
      case kBottomLeft:
        FRAME_Y = WINDOW_HEIGHT - FRAME_HEIGHT;
        break;
      case kTopLeft:
      default:
        break;
    }
  }

  void Frame::PreRender(nk::Context* ctx) {
    if(!nk_begin(ctx, name_.c_str(), nk_rect(FRAME_X, FRAME_Y, FRAME_WIDTH, FRAME_HEIGHT), flags_))
      LOG(FATAL) << "failed to render " << (*this);
  }

  void Frame::PostRender(nk::Context* ctx) {
    nk_end(ctx);
  }
}