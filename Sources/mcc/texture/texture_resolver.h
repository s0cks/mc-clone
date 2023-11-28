#ifndef MCC_TEXTURE_RESOLVER_H
#define MCC_TEXTURE_RESOLVER_H

#include <string>
#include <optional>

#include "mcc/uri.h"
#include "mcc/common.h"

namespace mcc::texture {
  class TextureResolver {
  protected:
    std::string root_;

    static inline bool
    HasExtension(const uri::Uri& uri) {
      const auto dotpos = uri.path.find_last_of('.');
      return dotpos != std::string::npos;
    }
  public:
    explicit TextureResolver(const std::string& root):
      root_(root) {  
    }
    virtual ~TextureResolver() = default;

    virtual std::optional<std::string> Resolve(const uri::Uri& uri) const;
  };
}

#endif //MCC_TEXTURE_RESOLVER_H