#include "mcc/fbo/fbo_attachment.h"

namespace mcc::fbo {
  std::string StencilAttachment::ToString() const {
    std::stringstream ss;
    ss << "StencilAttachment(";
    ss << ")";
    return ss.str();
  }

  StencilAttachment* StencilAttachment::New() {
    NOT_IMPLEMENTED(FATAL);//TODO: implement
    return nullptr;
  }
}