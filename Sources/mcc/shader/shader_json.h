#ifndef MCC_SHADER_JSON_H
#define MCC_SHADER_JSON_H

#include "mcc/json.h"
#include "mcc/json_spec.h"
#include "mcc/shader/shader_type.h"

namespace mcc {
  namespace shader {
    class Shader;
  }
  using shader::Shader;

  namespace json {
    class ShaderValue {
    protected:
      shader::ShaderType type_;
      const Value* value_;

      inline const Value* value() const {
        return value_;
      }
    public:
      explicit ShaderValue(const shader::ShaderType type,
                           const Value* value):
        type_(type),
        value_(value) {
        MCC_ASSERT(value);
      }
      virtual ~ShaderValue() = default;

      shader::ShaderType GetType() const {
        return type_;
      }

#define DEFINE_TYPE_CHECK(Name, Ext, GlValue)           \
      inline bool Is##Name##Shader() const {            \
        return GetType() == shader::k##Name##Shader;    \
      }
      FOR_EACH_SHADER_TYPE(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK

      inline bool IsString() const {
        return value()->IsString();
      }

      inline bool IsObject() const {
        return value()->IsObject();
      }

      virtual Shader* GetShader() const;

      friend std::ostream& operator<<(std::ostream& stream, const ShaderValue& rhs) {
        stream << "json::ShaderValue(";
        stream << "type=" << rhs.GetType();
        stream << ")";
        return stream;
      }
    };
  }
}

#endif //MCC_SHADER_JSON_H