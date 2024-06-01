#ifndef MCC_SHADER_H
#error "Please #include <mcc/shader/shader.h> instead."
#endif //MCC_SHADER_H

#ifndef MCC_SHADER_FRAGMENT_H
#define MCC_SHADER_FRAGMENT_H

#include "mcc/shader/shader.h"

namespace mcc::shader {
  class FragmentShader : public ShaderTemplate<kFragmentShader> {
  protected:
    explicit FragmentShader(const ShaderId id):
      ShaderTemplate<kFragmentShader>(id) {  
    }
  public:
    ~FragmentShader() override = default;
    DECLARE_SHADER_TYPE(Fragment);
  public:
    static FragmentShader* FromSource(ShaderCode* code);
    static FragmentShader* FromSource(const uri::Uri& uri);
    static FragmentShader* FromJson(const uri::Uri& uri);
    static FragmentShader* FromJson(const std::string& value);

    static inline FragmentShader*
    FromSource(const uint8_t* bytes, const uword num_bytes) {
      return FromSource(ShaderCode::NewFragmentShaderCode(bytes, num_bytes));
    }

    static inline FragmentShader*
    FromSource(const char* source, const uword length) {
      if(StartsWith(source, length, "file:")
      || StartsWith(source, length, "shader:")) {
        return FromSource(uri::Uri(std::string(source, length)));
      }
      return FromSource(ShaderCode::NewFragmentShaderCode(source, length));
    }

    static inline FragmentShader*
    FromSource(const char* source) {
      return FromSource(source, strlen(source));
    }

    static inline FragmentShader*
    FromSource(const std::string& source) {
      return FromSource(source.data(), source.length());
    }
  };
}

#endif //MCC_SHADER_FRAGMENT_H