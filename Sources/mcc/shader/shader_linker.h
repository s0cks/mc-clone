#ifndef MCC_SHADER_LINKER_H
#define MCC_SHADER_LINKER_H

#include "mcc/rx.h"
#include "mcc/shader/shader_constants.h"

namespace mcc::shader {
  class ShaderLinker {
    friend class ShaderLinkerTest;
  protected:
    static ShaderId LinkShader(const ShaderId dst, const ShaderId src);
    static ShaderId LinkProgram(const ShaderId result);
  public:
    ShaderLinker() = default;
    virtual ~ShaderLinker() = default;

    virtual rx::observable<ShaderId> Link(const ShaderId id, rx::observable<ShaderId> shaders) {
      if(id == kInvalidShaderId)
        return rx::observable<>::error<ShaderId>(std::runtime_error(""));
      DLOG(INFO) << "linking " << id << "....";
      return shaders.reduce(id, &LinkShader, &LinkProgram);
    }

    inline rx::observable<ShaderId> Link(rx::observable<ShaderId> shaders) {
      return Link(glCreateProgram(), shaders);
    }
  };
}

#endif //MCC_SHADER_LINKER_H