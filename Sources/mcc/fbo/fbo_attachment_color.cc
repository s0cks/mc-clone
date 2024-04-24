#include "mcc/fbo/fbo_attachment.h"

#include "mcc/texture/texture_factory.h"

namespace mcc::fbo {
  FboColorAttachment* FboColorAttachment::New(const glm::i32vec2& size) {
    using namespace texture;
    TextureFactory2D factory;
  }
}