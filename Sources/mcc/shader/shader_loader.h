#ifndef MCC_SHADER_LOADER_H
#define MCC_SHADER_LOADER_H

#include "mcc/shader/shader.h"
#include "mcc/resource.h"

namespace mcc::shader {
  class ShaderLoader {
  protected:
    const resource::ShaderRef& reference_;

    explicit ShaderLoader(const resource::ShaderRef& reference):
      reference_(reference) {  
    }
  public:
    virtual ~ShaderLoader() = default;
    virtual Shader LoadShader() = 0;

    const resource::ShaderRef& reference() const {
      return reference_;
    }
  };

  class JsonShaderLoader : public ShaderLoader {
  private:
    const json::Document& doc_;

    JsonShaderLoader(const resource::ShaderRef& reference, const json::Document& doc):
      ShaderLoader(reference),
      doc_(doc) {
    }
  public:
    ~JsonShaderLoader() override = default;

    const json::Document& doc() const {
      return doc_;
    }

    Shader LoadShader() override {
      return kUnknownShaderId;
    }
  };

  class DefaultShaderLoader : public ShaderLoader {
  private:
    DefaultShaderLoader(const resource::ShaderRef& reference):
      ShaderLoader(reference) {
    }
  public:
    ~DefaultShaderLoader() override = default;

    Shader LoadShader() override {
      DLOG(INFO) << "loading " << reference_ << "....";
      return Shader::Get(reference_.location);
    }
  };
}

#endif //MCC_SHADER_LOADER_H