#include "mcc/texture/texture.h"
#include <sstream>

namespace mcc::texture {
  std::string Texture3d::ToString() const {
    std::stringstream ss;
    ss << "Texture3d(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  TextureRef Texture3d::New(const uri::Uri& uri) {
    NOT_IMPLEMENTED(FATAL);
    return {};
  }
}