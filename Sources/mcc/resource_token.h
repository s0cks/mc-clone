#ifndef MCC_RESOURCE_H
#error "Please #include <mcc/resource.h> instead."
#endif //MCC_RESOURCE_H

#ifndef MCC_RESOURCE_TOKEN_H
#define MCC_RESOURCE_TOKEN_H

#include "mcc/flags.h"
#include "mcc/common.h"
#include "mcc/resource_tag.h"

namespace mcc::resource {
  struct Token {
    Tag tag;
    std::string location;

    Token():
      tag(),
      location() {
    }
    Token(const Tag& t, const std::string& l):
      tag(t),
      location(l) {
    }
    Token(const Token& rhs) = default;
    ~Token() = default;

    bool valid() const {
      return tag.type() != Type::kUnknownType
          && FileExists(FLAGS_resources + location);
    }

    Token& operator=(const Token& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const Token& rhs) {
      stream << "resource::Token(";
      stream << "tag=" << rhs.tag << ", ";
      stream << "location=" << rhs.location;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_RESOURCE_TOKEN_H