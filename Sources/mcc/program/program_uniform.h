#ifndef MCC_PROGRAM_UNIFORM_H
#define MCC_PROGRAM_UNIFORM_H

#include "mcc/gfx.h"
#include "mcc/common.h"

namespace mcc::program {
  class ProgramUniform {
    DEFINE_DEFAULT_COPYABLE_TYPE(ProgramUniform);
  protected:
    uword index_;
    GLenum type_;
    uword size_;
    std::string name_;
  public:
    ProgramUniform() = default;
    ProgramUniform(const GLint index,
                   const GLenum type,
                   const GLint size,
                   const char* name,
                   const int name_length):
      index_(static_cast<uword>(index)),
      type_(type),
      size_(static_cast<uword>(size)),
      name_(name, name_length) {
    }
    ~ProgramUniform() = default;

    uword GetIndex() const {
      return index_;
    }

    GLenum GetType() const {
      return type_;
    }

    const uword GetSize() const {
      return size_;
    }

    const std::string& GetName() const {
      return name_;
    }

    bool operator==(const ProgramUniform& rhs) const {
      return index_ == rhs.index_
          && type_ == rhs.type_
          && size_ == rhs.size_
          && name_ == rhs.name_;
    }

    bool operator!=(const ProgramUniform& rhs) const {
      return index_ != rhs.index_
          || type_ != rhs.type_
          || size_ != rhs.size_
          || name_ != rhs.name_;
    }

    bool operator<(const ProgramUniform& rhs) const {
      return index_ < rhs.index_;
    }

    bool operator>(const ProgramUniform& rhs) const {
      return index_ > rhs.index_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const ProgramUniform& rhs) {
      stream << "ProgramUniform(";
      stream << "index=" << rhs.GetIndex() << ", ";
      stream << "type=" << rhs.GetType() << ", ";
      stream << "size=" << rhs.GetSize() << ", ";
      stream << "name=" << rhs.GetName();
      stream << ")";
      return stream;
    }
  };
}

namespace fmt {
  using mcc::program::ProgramUniform;

  template<>
  struct formatter<ProgramUniform> : formatter<std::string_view> {
    auto format(const ProgramUniform& uniform, format_context& ctx) const {
      return formatter<std::string_view>::format(uniform.GetName(), ctx);
    }
  };
}

#endif //MCC_PROGRAM_UNIFORM_H