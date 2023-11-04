#ifndef MCC_SHADER_COMPILER_RESULT_H
#define MCC_SHADER_COMPILER_RESULT_H

#include <string>
#include <glog/logging.h>

namespace mcc::shader {
  class CompilerResult {
  protected:
    bool success_;
    std::string message_;
  public:
    CompilerResult():
      success_(false),
      message_() {
    }
    CompilerResult(const bool success,
                      const std::string& message):
      success_(success),
      message_(message) {
    }
    CompilerResult(const CompilerResult& rhs):
      success_(rhs.success_),
      message_(rhs.message_) {
    }
    ~CompilerResult() = default;

    bool success() const {
      return success_;
    }

    std::string message() const {
      return message_;
    }

    explicit operator bool() const {
      return success_;
    }

    void operator=(const CompilerResult& rhs) {
      success_ = rhs.success_;
      message_ = rhs.message_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const CompilerResult& rhs)  {
      stream << "CompilerResult(";
      stream << "success=" << rhs.success_ << ", ";
      stream << "message=" << rhs.message_;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_SHADER_COMPILER_RESULT_H