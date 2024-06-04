#ifndef MCC_PROGRAM_ATTRIBUTE_H
#define MCC_PROGRAM_ATTRIBUTE_H

#include <fmt/format.h>
#include "mcc/gfx.h"

namespace mcc::program {
  class ProgramAttribute {
    DEFINE_DEFAULT_COPYABLE_TYPE(ProgramAttribute);
  protected:
    uword index_;
    GLenum type_;
    uword size_;
    std::string name_;
  public:
    ProgramAttribute() = default;
    ProgramAttribute(const GLint index,
                     const GLenum type,
                     const GLint size,
                     const char* name,
                     const int name_length):
      index_(static_cast<uword>(index)),
      type_(type),
      size_(static_cast<uword>(size)),
      name_(name, name_length) {  
    }
    ~ProgramAttribute() = default;

    uword GetIndex() const {
      return index_;
    }

    GLenum GetType() const {
      return type_;
    }

    uword GetSize() const {
      return size_;
    }

    const std::string& GetName() const {
      return name_;
    }


    bool operator==(const ProgramAttribute& rhs) const {
      return index_ == rhs.index_
          && type_ == rhs.type_
          && size_ == rhs.size_
          && name_ == rhs.name_;
    }

    bool operator!=(const ProgramAttribute& rhs) const {
      return index_ != rhs.index_
          || type_ != rhs.type_
          || size_ != rhs.size_
          || name_ != rhs.name_;
    }

    bool operator<(const ProgramAttribute& rhs) const {
      return index_ < rhs.index_;
    }

    bool operator>(const ProgramAttribute& rhs) const {
      return index_ > rhs.index_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const ProgramAttribute& rhs) {
      stream << "ProgramAttribute(";
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
  using mcc::program::ProgramAttribute;

  template<>
  struct formatter<ProgramAttribute> : formatter<std::string_view> {
    auto format(const ProgramAttribute& uniform, format_context& ctx) const {
      return formatter<std::string_view>::format(uniform.GetName(), ctx);
    }
  };
}


#endif //MCC_PROGRAM_ATTRIBUTE_H