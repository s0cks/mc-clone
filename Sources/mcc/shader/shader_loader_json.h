#ifndef MCC_SHADER_LOADER_JSON_H
#define MCC_SHADER_LOADER_JSON_H

#include "mcc/shader/shader_type.h"
#include "mcc/shader/shader_json.h"
#include "mcc/shader/shader_loader.h"

namespace mcc::shader {
  class JsonShaderLoader : public ShaderLoader {
  protected:
    ShaderType type_;
    const json::Value& value_;

    inline const json::Value& value() const {
      return value_;
    }

    std::string GetShaderSource() const;

    Shader* LoadShaderFromUri(const uri::Uri& uri) const;
    Shader* LoadShaderFromSource(const std::string& source) const;
  public:
    explicit JsonShaderLoader(const ShaderType type,
                              const json::Value& value):
      ShaderLoader(),
      type_(type),
      value_(value) {
    }
    ~JsonShaderLoader() override = default;
    Shader* LoadShader() const override;
  };
}

#endif //MCC_SHADER_LOADER_JSON_H