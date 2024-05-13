#ifndef MCC_SHADER_H
#error "Please #include <mcc/shader/shader.h> instead."
#endif //MCC_SHADER_H

#ifndef MCC_SHADER_TESS_EVAL_H
#define MCC_SHADER_TESS_EVAL_H

#include "mcc/shader/shader.h"

namespace mcc::shader {
  class TessEvalShader : public ShaderTemplate<kTessEvalShader> {
  protected:
    explicit TessEvalShader(const ShaderId id):
      ShaderTemplate<kTessEvalShader>(id) {  
    }
  public:
    ~TessEvalShader() override = default;
    DECLARE_SHADER_TYPE(TessEval);
  };
}

#endif //MCC_SHADER_TESS_EVAL_H