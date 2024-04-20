#ifndef MCC_SHADER_STATUS_H
#define MCC_SHADER_STATUS_H

#include "mcc/shader/shader_constants.h"

namespace mcc::shader {
  class ShaderStatus {
  protected:
    ShaderId shader_;
    uint32_t message_len_;
    uint8_t* message_;

    explicit ShaderStatus(const ShaderId id):
      shader_(id),
      message_len_(0),
      message_(nullptr) {
    }

    inline void SetMessage(uint8_t* bytes, const uint32_t nbytes) {
      message_ = bytes;
      message_len_ = nbytes;
    }

    inline void CopyMessage(const ShaderStatus& rhs) {
      MCC_ASSERT(rhs.HasMessage());
      const auto length = rhs.message_len_;
      const auto total_size = sizeof(uint8_t) * length;
      auto message = (uint8_t*) malloc(total_size);
      LOG_IF(FATAL, !message) << "failed to allocate ShaderStatus message for copy.";
      memcpy(message, rhs.message_, total_size);
      message_ = message;
      message_len_ = length;
    }

    inline void FreeMessage() {
      MCC_ASSERT(HasMessage());
      free(message_);
      message_ = nullptr;
      message_len_ = 0;
    }
  public:
    ShaderStatus():
      shader_(kInvalidShaderId),
      message_len_(0),
      message_(nullptr) {
    }
    ShaderStatus(const ShaderStatus& rhs):
      shader_(rhs.shader_),
      message_len_(0),
      message_(nullptr) {
      if(rhs.HasMessage())
        CopyMessage(rhs);
    }
    virtual ~ShaderStatus() {
      if(HasMessage())
        FreeMessage();
    }

    ShaderId shader() const {
      return shader_;
    }

    std::string message() const {
      return std::string((const char*) message_, message_len_);
    }

    bool HasMessage() const {
      return message_ != nullptr && message_len_ > 0;
    }

    void operator=(const ShaderStatus& rhs) {
      if(HasMessage()) 
        FreeMessage();
      shader_ = rhs.shader_;
      if(rhs.HasMessage())
        CopyMessage(rhs);
    }
  };

  class ShaderCompileStatus : public ShaderStatus {
  protected:
    GLint compiled_;
  public:
    explicit ShaderCompileStatus(const ShaderId id):
      ShaderStatus(id),
      compiled_(GL_FALSE) {
      glGetShaderiv(shader(), GL_COMPILE_STATUS, &compiled_);
      CHECK_GL(FATAL);
      if(!IsCompiled()) {
        GLint length = 0;
        glGetShaderiv(shader(), GL_INFO_LOG_LENGTH, &length);
        CHECK_GL(FATAL);
        auto data = (uint8_t*) malloc(sizeof(uint8_t) * length);
        LOG_IF(FATAL, !data) << "failed to allocate ShaderCompileStatus message of length: " << length;
        glGetShaderInfoLog(shader(), length, &length, (GLchar*) data);
        CHECK_GL(FATAL);
        SetMessage(data, length);
      }
    }
    ~ShaderCompileStatus() override = default;

    bool IsCompiled() const {
      return compiled_ == GL_TRUE;
    }

    operator bool() const {
      return IsCompiled();
    }

    friend std::ostream& operator<<(std::ostream& stream, const ShaderCompileStatus& rhs) {
      stream << "ShaderCompileStatus(";
      stream << "shader=" << rhs.shader();
      if(rhs.HasMessage())
        stream << ", message=" << rhs.message();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_SHADER_STATUS_H