#ifndef MCC_SHADER_SOURCE_H
#define MCC_SHADER_SOURCE_H

#include <memory>
#include <fmt/format.h>

#include "mcc/gfx.h"
#include "mcc/flags.h"
#include "mcc/buffer.h"
#include "mcc/common.h"

#include "mcc/rx.h"
#include "mcc/shader/shader_constants.h"

namespace mcc::shader {
  class ShaderCode;
  typedef std::shared_ptr<ShaderCode> ShaderCodePtr;

  class ShaderSource {
  protected:
    ShaderType type_;
    std::string path_;
  public:
    ShaderSource(const ShaderType type,
                 const std::string& path):
      type_(type),
      path_(path) {
    }
    ShaderSource(const ShaderSource& rhs) = default;
    virtual ~ShaderSource() = default;

    ShaderType type() const {
      return type_;
    }

    std::string path() const {
      return path_;
    }

    std::string filename() const {
      const auto last_slash = path_.find_last_of('/');
      if(last_slash == std::string::npos)
        return path_;
      return path_.substr(last_slash + 1);
    }

    ShaderCodePtr GetSource() const;

    ShaderSource& operator=(const ShaderSource& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const ShaderSource& rhs) {
      stream << "ShaderSource(";
      stream << "type=" << rhs.type() << ", ";
      stream << "filename=" << rhs.filename();
      stream << ")";
      return stream;
    }
  };

  class ShaderCode {
  protected:
    ShaderType type_;
    BufferPtr data_;
    const ShaderSource* source_;
  public:
    ShaderCode(const ShaderType type, const std::string& code):
      type_(type),
      data_(Buffer::CopyFrom(code)),
      source_(nullptr) {
    }
    ShaderCode(const ShaderType type, const char* code):
      ShaderCode(type, std::string(code)) {
    }
    ShaderCode(const ShaderType type, BufferPtr code):
      type_(type),
      data_(code),
      source_(nullptr) {
    }
    ShaderCode(const ShaderSource* source, BufferPtr code):
      type_(source->type()),
      data_(code),
      source_(source) {
    }
    explicit ShaderCode(const ShaderSource* source):
      ShaderCode(source, Buffer::FromFile(source->path())) {
    }
    ShaderCode(const ShaderCode& rhs) = default;
    virtual ~ShaderCode() = default;

    inline ShaderType type() const {
      return type_;
    }

    inline BufferPtr raw() const {
      return data_;
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

    ShaderCode& operator=(const ShaderCode& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const ShaderCode& rhs) {
      stream << "ShaderCode(";
      stream << "type=" << rhs.type() << ", ";
      stream << "length=" << rhs.length();
      stream << ")";
      return stream;
    }
  public:
    static inline ShaderCodePtr
    New(const ShaderType type, const std::string& code) {
      return std::make_shared<ShaderCode>(type, code);
    }
  };
}

#endif//MCC_SHADER_SOURCE_H