#ifndef MCC_SHADER_COMPILER_STATUS_H
#define MCC_SHADER_COMPILER_STATUS_H

#include "mcc/gfx.h"
#include "mcc/shader/shader_id.h"

namespace mcc::shader {
  struct ShaderCompilerStatus {
    ShaderId id;
    bool compiled;
    std::string message;

    ShaderCompilerStatus(const ShaderId id);
    ShaderCompilerStatus(const ShaderCompilerStatus& rhs) = default;
    ~ShaderCompilerStatus() = default;

    bool HasMessage() const {
      return !message.empty();
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
        return stream << "Compilation Error: " << rhs.message;
      }
      return stream << "Unknown ShaderCompilerStatus: " << rhs.ToString();
    }
  };
}

#endif //MCC_SHADER_COMPILER_STATUS_H