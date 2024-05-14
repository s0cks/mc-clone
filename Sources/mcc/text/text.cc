#include "mcc/text/text.h"
#include <glog/logging.h>

namespace mcc::text {
  static TextSet all_;

  static inline void
  Insert(const Text* rhs) {
    const auto pos = all_.insert(rhs);
    LOG_IF(WARNING, !pos.second) << "failed to register: " << (*rhs);
  }

  static inline void
  Remove(const Text* rhs) {
    const auto removed = all_.erase(rhs);
    LOG_IF(WARNING, removed != 1) << "failed to remove " << (*rhs) << " " << removed << "/1";
  }

  const TextSet& GetAll() {
    return all_;
  }

  Text::Text(Font* font, const std::string& value, const glm::vec2& pos):
    Object(),
    font_(font),
    value_(value),
    pos_(pos) {
    Insert(this);
  }

  Text::~Text() {
    Remove(this);
  }

  std::string Text::ToString() const {
    std::stringstream ss;
    ss << "Text(";
    ss << "value=" << GetValue() << ", ";
    ss << "pos=" << glm::to_string(GetPos()) << ", ";
    ss << "font=" << GetFont()->ToString();
    ss << ")";
    return ss.str();
  }
}