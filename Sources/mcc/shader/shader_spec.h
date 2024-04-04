#ifndef MCC_SHADER_SPEC_H
#define MCC_SHADER_SPEC_H

#include <string>
#include <optional>

#include "mcc/uri.h"
#include "mcc/spec.h"

namespace mcc::shader {
  class ShaderSpec : public Spec {
  protected:
    ShaderSpec() = default;
  public:
    ~ShaderSpec() override = default;
    DEFINE_SPEC(Shader);
    DECLARE_SPEC_PROPERTY(FragmentShader, uri::Uri);
    DECLARE_SPEC_PROPERTY(VertexShader, uri::Uri);
    DECLARE_SPEC_PROPERTY(GeometryShader, uri::Uri);
    DECLARE_SPEC_PROPERTY(TessEvalShader, uri::Uri);
    DECLARE_SPEC_PROPERTY(TessControlShader, uri::Uri);
  };
}

#endif //MCC_SHADER_SPEC_H