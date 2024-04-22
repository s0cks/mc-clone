#ifndef MCC_UV_STATUS_H
#define MCC_UV_STATUS_H

#include <uv.h>

namespace mcc::uv {
#ifndef UV_OK
#define UV_OK 0
#endif //UV_OK

  typedef int StatusId;
  class Status {
  private:
    StatusId id_;
  public:
    constexpr Status(const StatusId id):
      id_(id) {
    }
    Status(const Status& rhs) = default;
    ~Status() = default;

    inline StatusId id() const {
      return id_;
    }

    inline bool IsOk() const {
      return id() == UV_OK;
    }

    const char* message() const {
      return IsOk() ? "Ok" : uv_strerror(id());
    }
    
    operator StatusId () const {
      return id();
    }

    operator bool () const {
      return IsOk();
    }

    Status& operator=(const Status& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const Status& rhs) {
      stream << "uv::Status(";
      stream << "id=" << rhs.id() << ", ";
      stream << "is_ok=" << rhs.IsOk();
      if(!rhs)
        stream << ", message=\"" << rhs.message() << "\"";
      stream << ")";
      return stream;
    }
  public:
    static inline constexpr Status
    New(const StatusId raw) {
      return Status(raw);
    }

    static inline constexpr Status
    Ok() {
      return New(UV_OK);
    }
  };
}

#endif //MCC_UV_STATUS_H