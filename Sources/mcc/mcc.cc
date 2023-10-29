#include <sstream>
#include <glog/logging.h>

#include "mcc/mcc.h"

namespace mcc {
  std::string GetVersion(){
    std::stringstream ss;
    ss << MCC_VERSION_MAJOR << ".";
    ss << MCC_VERSION_MINOR << ".";
    ss << MCC_VERSION_PATCH;
    return ss.str();
  }

  class GlfwError {
  public:
    int code_;
    std::string message_;
  public:
    GlfwError(const int code, const char* description):
      code_(code),
      message_(description) {
    }
    ~GlfwError() = default;

    std::string name() const {
      return "GlfwError";
    }

    std::string message() const {
      return message_;
    }

    int code() const {
      return code_;
    }

    google::LogSeverity severity() const {
      switch(code_) {
        default:
          return google::FATAL;
      }
    }

    friend std::ostream& operator<<(std::ostream& stream, const GlfwError& rhs) {
      stream << rhs.name() << "[" << rhs.code() << "]: " << rhs.message();
      return stream;
    }
  };

  void OnGlfwError(const int code, const char* description) {
    GlfwError error(code, description);
    LOG_AT_LEVEL(error.severity()) << "unexpected " << error;
  }
}