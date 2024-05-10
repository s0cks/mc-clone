#include "mcc/shader/shader_json.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/shader_uri.h"
#include "mcc/shader/shader_code.h"

namespace mcc::json {
  Shader* ShaderValue::GetShader() const {
    if(IsString()) {
      const auto val = std::string(value()->GetString(), value()->GetStringLength());
      if(shader::IsValidShaderUri(val)) {
        NOT_IMPLEMENTED(FATAL); //TODO: implement
        return nullptr; //TODO: load shader from uri
      }

      NOT_IMPLEMENTED(FATAL); //TODO: implement
      return nullptr; //TODO: load shader from raw code
    } else if(IsObject()) {
      NOT_IMPLEMENTED(FATAL); //TODO: implement
    }

    LOG(ERROR) << "invalid json type for " << (*this);
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }
}