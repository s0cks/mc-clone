#ifndef MCC_SHADER_RESOLVER_H
#define MCC_SHADER_RESOLVER_H

#include <vector>
#include <string>

#include "mcc/uri.h"

namespace mcc {
  class ShaderResolver {
  protected:
    std::string root_;
    std::vector<std::string>& results_;
  public:
    explicit ShaderResolver(const std::string& root,
                            std::vector<std::string>& results):
      root_(root),
      results_(results) {
    }
    virtual ~ShaderResolver() = default;

    bool Resolve(const uri::Uri& target);
  };
}

#endif //MCC_SHADER_RESOLVER_H