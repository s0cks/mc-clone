#ifndef MCC_TEXT_H
#define MCC_TEXT_H

#include "mcc/glm.h"
#include "mcc/font/font.h"

namespace mcc::text {
  using mcc::font::Font;

  class Text {
  protected:
    Font* font_;
    std::string value_;
    glm::vec2 pos_;
  public:
    Text(Font* font, const std::string& value, const glm::vec2& pos):
      font_(font),
      value_(value),
      pos_(pos) {
    }
    ~Text() = default;

    Font* GetFont() const {
      return font_;
    }

    const std::string& GetValue() const {
      return value_;
    }

    const glm::vec2& GetPos() const {
      return pos_;
    }
  };
}

#endif //MCC_TEXT_H