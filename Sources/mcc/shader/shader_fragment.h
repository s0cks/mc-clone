#ifndef MCC_SHADER_H
#error "Please #include <mcc/shader/shader.h> instead."
#endif //MCC_SHADER_H

#ifndef MCC_SHADER_FRAGMENT_H
#define MCC_SHADER_FRAGMENT_H

#include "mcc/shader/shader.h"

namespace mcc::shader {
  class FragmentShader : public ShaderTemplate<kFragmentShader> {
  protected:
    explicit FragmentShader(const Metadata& meta, const ShaderId id):
      ShaderTemplate<kFragmentShader>(meta, id) {  
    }
  public:
    ~FragmentShader() override = default;
    DECLARE_SHADER_TYPE(Fragment);
  public:
    static FragmentShader* FromSource(const uri::Uri& uri);
    static FragmentShader* FromJson(const uri::Uri& uri);
    static FragmentShader* FromJson(const std::string& value);
  };
}

#endif //MCC_SHADER_FRAGMENT_H