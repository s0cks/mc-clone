#ifndef MCC_SHADER_H
#error "Please #include <mcc/shader/shader.h> instead."
#endif //MCC_SHADER_H

#ifndef MCC_SHADER_GEOMETRY_H
#define MCC_SHADER_GEOMETRY_H

#include "mcc/shader/shader.h"

namespace mcc::shader {
  class GeometryShader : public ShaderTemplate<kGeometryShader> {
  protected:
    explicit GeometryShader(const Metadata& meta, const ShaderId id):
      ShaderTemplate<kGeometryShader>(meta, id) {
    }
  public:
    ~GeometryShader() override = default;
    DECLARE_SHADER_TYPE(Geometry);
  };
}

#endif //MCC_SHADER_GEOMETRY_H