#ifndef MCC_TEXTURE_FILE_RESOLVER_H
#define MCC_TEXTURE_FILE_RESOLVER_H

#include "mcc/uri.h"

namespace mcc::texture {
  class TextureFileResolver {
  protected:
    std::string dir_;
    uri::Uri target_;

    TextureFileResolver(const std::string& dir, const uri::Uri& target);
  public:
    virtual ~TextureFileResolver() = default;

    uri::Uri target() const {
      return target_;
    }

    std::string dir() const {
      return dir_;
    }
  };
}

#endif //MCC_TEXTURE_FILE_RESOLVER_H