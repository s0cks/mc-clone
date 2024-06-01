#ifndef MCC_TAG_H
#define MCC_TAG_H

#include <string>
#include <vector>
#include <unordered_set>
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

    bool operator==(const RawTag& rhs) const {
      return raw() == rhs;
    }

    bool operator!=(const Tag& rhs) const {
      return raw() != rhs.raw();
    }

    bool operator!=(const RawTag& rhs) const {
      return raw() != rhs;
    }

    bool operator<(const Tag& rhs) const {
      return raw() < rhs.raw();
    }

    bool operator<(const RawTag& rhs) const {
      return raw() < rhs;
    }

    bool operator>(const Tag& rhs) const {
      return raw() > rhs.raw();
    }

    bool operator>(const RawTag& rhs) const {
      return raw() > rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Tag& rhs) {
      stream << "Tag(";
      stream << "raw=" << rhs.raw();
      stream << ")";
      return stream;
    }
  };
}

namespace std {
  using mcc::Tag;

  template<>
  struct hash<Tag> {
    size_t operator()(const Tag& value) const {
      size_t h = 0;
      combine<std::string>(h, value.raw());
      return h;
    }
private:
    template<typename T>
    static inline void
    combine(size_t& hash, const T& value) {
      std::hash<T> hasher;
      hash ^= hasher(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
  };
}

namespace mcc {
  typedef std::unordered_set<Tag> TagSet;
  typedef std::vector<Tag> TagList;

  static inline std::ostream&
  operator<<(std::ostream& stream, const TagSet& rhs) {
    stream << "[";
    auto idx = 0;
    for(const auto& tag : rhs) {
      stream << tag;
      if(idx++ < (rhs.size() - 1))
        stream << ", ";
    }
    stream << "]";
    return stream;
  }

  static inline std::ostream&
  operator<<(std::ostream& stream, const TagList& rhs) {
    stream << "[";
    auto idx = 0;
    for(const auto& tag : rhs) {
      stream << tag;
      if(idx++ < (rhs.size() - 1))
        stream << ", ";
    }
    stream << "]";
    return stream;
  }
}

#endif //MCC_TAG_H