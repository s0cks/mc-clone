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
  public:
    virtual ~ShaderStatus() {
      if(message_)
        free(message_);
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

  class ShaderLinkStatus : public ShaderStatus {
  protected:
    GLint linked_;
  public:
    explicit ShaderLinkStatus(const ShaderId id):
      ShaderStatus(id),
      linked_(GL_FALSE) {
      glGetShaderiv(shader(), GL_LINK_STATUS, &linked_);
      CHECK_GL(FATAL);
      if(!IsLinked()) {
        GLint length = 0;
        glGetShaderiv(shader(), GL_INFO_LOG_LENGTH, &length);
        CHECK_GL(FATAL);
        auto data = (uint8_t*) malloc(sizeof(uint8_t) * length);
        LOG_IF(FATAL, !data) << "failed to allocate ShaderLinkStatus message of length: " << length;
        glGetShaderInfoLog(shader(), length, &length, (GLchar*) data);
        CHECK_GL(FATAL);
        SetMessage(data, length);
      }
    }
    ~ShaderLinkStatus() override = default;

    bool IsLinked() const {
      return linked_ == GL_TRUE;
    }

    operator bool () const {
      return IsLinked();
    }

    friend std::ostream& operator<<(std::ostream& stream, const ShaderLinkStatus& rhs) {
      stream << "ShaderLinkStatus(";
      stream << "shader=" << rhs.shader();
      if(rhs.HasMessage())
        stream << ", message=" << rhs.message();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_SHADER_STATUS_H