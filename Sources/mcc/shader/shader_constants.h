#ifndef MCC_SHADER_CONSTANTS_H
#define MCC_SHADER_CONSTANTS_H

#include <memory>
#include <cstdint>
#include <fmt/format.h>

#include "mcc/uri.h"
#include "mcc/gfx.h"

namespace mcc::shader {
  class InvalidShaderUriException : public std::exception {
  protected:
    uri::Uri uri_;
    std::string message_;
  public:
    explicit InvalidShaderUriException(const uri::Uri& uri):
      exception(),
      uri_(uri),
      message_(fmt::format("invalid shader uri: {0:s}", (const std::string&) uri)) {
    }
    ~InvalidShaderUriException() override = default;

    uri::Uri uri() const {
      return uri_;
    }

    std::string message() const {
      return message_;
    }

    const char* what() {
      return message_.data();
    }
  };
}

#endif //MCC_SHADER_CONSTANTS_H