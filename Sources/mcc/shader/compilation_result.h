#ifndef MCC_SHADER_COMPILATION_RESULT_H
#define MCC_SHADER_COMPILATION_RESULT_H

#include <string>
#include <glog/logging.h>

namespace mcc::shader {
  class CompilationResult {
  protected:
    bool success_;
    std::string message_;
  public:
    CompilationResult():
      success_(false),
      message_() {
    }
    CompilationResult(const bool success,
                      const std::string& message):
      success_(success),
      message_(message) {
    }
    CompilationResult(const CompilationResult& rhs):
      success_(rhs.success_),
      message_(rhs.message_) {
    }
    ~CompilationResult() = default;

    bool success() const {
      return success_;
    }

    std::string message() const {
      return message_;
    }

    explicit operator bool() const {
      return success_;
    }

    void operator=(const CompilationResult& rhs) {
      success_ = rhs.success_;
      message_ = rhs.message_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const CompilationResult& rhs)  {
      stream << "CompilationResult(";
      stream << "success=" << rhs.success_ << ", ";
      stream << "message=" << rhs.message_;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_SHADER_COMPILATION_RESULT_H