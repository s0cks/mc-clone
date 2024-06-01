#ifndef MCC_TAG_H
#define MCC_TAG_H

#include <string>
#include "mcc/common.h"

namespace mcc {
  typedef std::string RawTag;
  class Tag {
    DEFINE_DEFAULT_COPYABLE_TYPE(Tag);
  protected:
    RawTag raw_;
  public:
    Tag() = default;
    explicit Tag(const RawTag& raw):
      raw_(raw) {
    }
    ~Tag() = default;

    const RawTag& raw() const {
      return raw_;
    }

    operator std::string() const {
      return raw();
    }

    Tag& operator=(const RawTag& raw) {
      raw_ = raw;
      return *this;
    }

    bool operator==(const Tag& rhs) const {
      return raw() == rhs.raw();
    }

    bool operator!=(const Tag& rhs) const {
      return raw() != rhs.raw();
    }

    bool operator<(const Tag& rhs) const {
      return raw() < rhs.raw();
    }

    bool operator>(const Tag& rhs) const {
      return raw() > rhs.raw();
    }

    friend std::ostream& operator<<(std::ostream& stream, const Tag& rhs) {
      stream << "Tag(";
      stream << "raw=" << rhs.raw();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_TAG_H