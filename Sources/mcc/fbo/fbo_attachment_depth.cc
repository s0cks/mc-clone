#include "mcc/fbo/fbo_attachment.h"

namespace mcc::fbo {
  std::string DepthAttachment::ToString() const {
    std::stringstream ss;
    ss << "DepthAttachment(";
    ss << ")";
    return ss.str();
  }

  DepthAttachment* DepthAttachment::New() {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }
}