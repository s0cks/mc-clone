#ifndef MCC_SHADER_LOADER_H
#define MCC_SHADER_LOADER_H

namespace mcc::shader {
  class Shader;
  class ShaderLoader {
  protected:
    ShaderLoader() = default;
  public:
    virtual ~ShaderLoader() = default;
    virtual Shader* LoadShader() const = 0;
  };
}

#endif //MCC_SHADER_LOADER_H