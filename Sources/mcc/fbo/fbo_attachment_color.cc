#include "mcc/fbo/fbo_attachment.h"

#include <sstream>
#include "mcc/texture/texture_factory.h"

namespace mcc::fbo {
  std::string ColorAttachment::ToString() const {
    std::stringstream ss;
    ss << "fbo::ColorAttachment(";
    ss << "texture=" << GetTexture()->GetTextureId();
    ss << ")";
    return ss.str();
  }

  ColorAttachment* ColorAttachment::New(const glm::i32vec2& size) {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }
}