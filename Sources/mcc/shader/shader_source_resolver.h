#ifndef MCC_SHADER_SOURCE_RESOLVER_H
#define MCC_SHADER_SOURCE_RESOLVER_H

#include "mcc/uri.h"
#include "mcc/shader/source.h"

namespace mcc::shader {
  class ShaderSourceResolver {
  private:
    std::string dir_;
    uri::Uri target_;

    inline uri::Uri& target() {
      return target_;
    }

    rx::observable<ShaderSource> ResolveFromJsonFile(const std::string& jsonf);
  public:
    ShaderSourceResolver(const std::string& dir, const uri::Uri& target);
    explicit ShaderSourceResolver(const uri::Uri& target);
    virtual ~ShaderSourceResolver() = default;
    virtual rx::observable<ShaderSource> Resolve();
  };
}

#endif //MCC_SHADER_SOURCE_RESOLVER_H