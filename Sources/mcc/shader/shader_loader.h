#ifndef MCC_SHADER_LOADER_H
#define MCC_SHADER_LOADER_H

#include "mcc/shader/shader.h"

namespace mcc {
  class ShaderLoader {
  protected:
    const uri::Uri& target_;

    explicit ShaderLoader(const uri::Uri& target):
      target_(target) {
    }
  public:
    virtual ~ShaderLoader() = default;
    virtual ShaderRef Load() = 0;

    const uri::Uri& target() const {
      return target_;
    }
  };
}

#endif //MCC_SHADER_LOADER_H