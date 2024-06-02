#ifndef MCC_SHADER_H
#error "Please #include <mcc/shader/shader.h> instead."
#endif //MCC_SHADER_H

#ifndef MCC_SHADER_VERTEX_H
#define MCC_SHADER_VERTEX_H

#include "mcc/shader/shader.h"

namespace mcc::shader {
  class VertexShader : public ShaderTemplate<kVertexShader> {
  protected:
    explicit VertexShader(const ShaderId id):
      ShaderTemplate<kVertexShader>(id) {  
    }
  public:
    ~VertexShader() override = default;
    DECLARE_SHADER_TYPE(Vertex);
  public:
    static VertexShader* New(ShaderUnit* unit);
    static VertexShader* FromSource(const uri::Uri& uri);
    static VertexShader* FromJson(const uri::Uri& uri);
    static VertexShader* FromJson(const char* source, const uword length);

    static inline VertexShader*
    FromJson(const char* source) {
      return FromJson(source, strlen(source));
    }

    static inline VertexShader*
    FromJson(const std::string& source) {
      return FromJson(source.data(), source.length());
    }
  };
}

#endif //MCC_SHADER_VERTEX_H