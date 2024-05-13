#ifndef MCC_SHADER_H
#error "Please #include <mcc/shader/shader.h> instead."
#endif //MCC_SHADER_H

#ifndef MCC_SHADER_TESS_CONTROL_H
#define MCC_SHADER_TESS_CONTROL_H

#include "mcc/shader/shader.h"

namespace mcc::shader {
  class TessControlShader : public ShaderTemplate<kTessControlShader> {
  protected:
    explicit TessControlShader(const ShaderId id):
      ShaderTemplate<kTessControlShader>(id) {
    }
  public:
    ~TessControlShader() override = default;
    DECLARE_SHADER_TYPE(TessControl);
  };
}

#endif //MCC_SHADER_TESS_CONTROL_H