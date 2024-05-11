#ifndef MCC_SHADER_COMPILER_STATUS_H
#define MCC_SHADER_COMPILER_STATUS_H

#include "mcc/gfx.h"
#include "mcc/shader/shader_id.h"
#include "mcc/shader/shader_info_log.h"

namespace mcc::shader {
  struct ShaderCompilerStatus {
    ShaderId id;
    bool compiled;
    ShaderInfoLog info;

    ShaderCompilerStatus(const ShaderId id);
    ShaderCompilerStatus(const ShaderCompilerStatus& rhs) = default;
    ~ShaderCompilerStatus() = default;

    bool HasMessage() const {
      return !info.IsEmpty();
    }

    operator bool() const {
      return compiled;
    }

    std::string ToString() const;

    ShaderCompilerStatus& operator=(const ShaderCompilerStatus& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const ShaderCompilerStatus& rhs) {
      if(rhs.compiled) {
        return stream << "Compiled";
      } else if(!rhs.compiled && rhs.HasMessage()) {
        return stream << "Compilation Error: " << rhs.info;
      }
      return stream << "Unknown ShaderCompilerStatus: " << rhs.ToString();
    }
  };
}

#endif //MCC_SHADER_COMPILER_STATUS_H