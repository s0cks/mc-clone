#ifndef MCC_SHADER_INFO_LOG_H
#define MCC_SHADER_INFO_LOG_H

#include "mcc/common.h"
#include "mcc/platform.h"
#include "mcc/shader/shader_id.h"

namespace mcc::shader {
  class ShaderInfoLog {
    static uword GetShaderInfoLogLength(const ShaderId id);
    static void GetShaderInfoLogData(const ShaderId id, uint8_t* bytes, const uword max_size, uword* length);
  protected:
    ShaderId id_;
    uword length_;
    uint8_t* data_;

    void Resize(const uword num_bytes);

    inline void CopyFrom(const uint8_t* bytes, const uword num_bytes) {
      MCC_ASSERT(bytes);
      MCC_ASSERT(num_bytes >= 1);
      Resize(num_bytes);
      memcpy(data_, bytes, num_bytes);
    }

    inline void CopyFrom(const ShaderInfoLog& rhs) {
      return CopyFrom(rhs.data(), rhs.length());
    }

    inline void Release() {
      if(data_)
        free(data_);
      data_ = nullptr;
      length_ = 0;
    }
  public:
    ShaderInfoLog() = default;
    explicit ShaderInfoLog(const ShaderId id):
      id_(id),
      length_(0),
      data_(nullptr) {
      auto length = GetShaderInfoLogLength(id);
      if(length <= 0)
        return;
      Resize(length);
      GetShaderInfoLogData(id, data_, length, &length);
      LOG_IF(FATAL, length <= 0) << "failed to get ShaderInfoLog for shader: " << id;
    }
    ShaderInfoLog(const ShaderInfoLog& rhs):
      id_(rhs.id_),
      length_(0),
      data_(nullptr) {
      CopyFrom(rhs);
    }
    ~ShaderInfoLog() {
      Release();
    }

    ShaderId GetShaderId() const {
      return id_;
    }

    uword length() const {
      return length_;
    }

    const uint8_t* data() const {
      return data_;
    }

    bool IsEmpty() const {
      return data_ == nullptr || length_ == 0;
    }

    operator std::string() const {
      return std::string((const char*) data(), length());
    }

    void operator=(const ShaderInfoLog& rhs) {
      id_ = rhs.id_;
      CopyFrom(rhs);
    }

    friend std::ostream& operator<<(std::ostream& stream, const ShaderInfoLog& rhs) {
      return stream << ((const std::string&) rhs);
    }
  };
}

#endif //MCC_SHADER_INFO_LOG_H