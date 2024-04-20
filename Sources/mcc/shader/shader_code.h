#ifndef MCC_SHADER_CODE_H
#define MCC_SHADER_CODE_H

#include <ostream>
#include "mcc/buffer.h"
#include "mcc/shader/shader_type.h"

namespace mcc::shader {
  class ShaderCode {
    DEFINE_NON_COPYABLE_TYPE(ShaderCode);
  protected:
    ShaderType type_;
    BufferPtr data_;

    ShaderCode(const ShaderType type,
               BufferPtr data):
      type_(type),
      data_(data) {
    }

    inline BufferPtr raw() const {
      return data_;
    }
  public:
    ShaderCode() = delete;
    virtual ~ShaderCode() = default;

    ShaderType GetShaderType() const {
      return type_;
    }

    const GLchar* data() const {
      return (const GLchar*) raw()->data();
    }

    const GLint length() const {
      return (const GLint) raw()->write_pos();
    }

    bool IsEmpty() const {
      return data_->empty();
    }

    operator std::string () const {
      return std::string((const char*) data(), length());
    }

    friend std::ostream& operator<<(std::ostream& stream, const ShaderCode& rhs) {
      return stream << (const std::string&) rhs;
    }
  public:
    static inline ShaderCode* 
    New(const ShaderType type, BufferPtr data) {
      return new ShaderCode(type, data);
    }

    static inline ShaderCode*
    New(const ShaderType type, const std::string& data) {
      return new ShaderCode(type, Buffer::CopyFrom(data));
    }

    static inline ShaderCode*
    New(const ShaderType type, const json::Value& value) {
      MCC_ASSERT(value.IsString());
      std::string val(value.GetString(), value.GetStringLength());
      return New(type, val);
    }

    static inline ShaderCode*
    Wrap(const ShaderType type, uint8_t* bytes, const uint64_t nbytes) {
      return new ShaderCode(type, Buffer::Wrap(bytes, nbytes));
    }

    static inline ShaderCode*
    CopyFrom(const ShaderType type, const uint8_t* bytes, const uint64_t nbytes) {
      return new ShaderCode(type, Buffer::CopyFrom(bytes, nbytes));
    }

    static inline ShaderCode*
    FromFile(const ShaderType type, const uri::Uri& uri) {
      return new ShaderCode(type, Buffer::FromFile(uri));
    }

    static ShaderCode* FromFile(const uri::Uri& uri);
  };
}

#endif //MCC_SHADER_CODE_H