#ifndef MCC_SHADER_SOURCE_H
#define MCC_SHADER_SOURCE_H

#include "mcc/gfx.h"
#include "mcc/buffer.h"
#include "mcc/common.h"

namespace mcc::shader {
  class Source {
  private:
    BufferPtr data_;
  public:
    Source() = default;
    Source(const BufferPtr& data):
      data_(data) {
    }
    Source(const Source& rhs):
      data_(rhs.data_) {
    }
    ~Source() = default;

    BufferPtr buffer() const {
      return data_;
    }

    GLchar* data() const {
      return reinterpret_cast<GLchar*>(data_->data());
    }

    GLint size() const {
      return static_cast<GLint>(data_->write_pos());
    }

    void operator=(const Source& rhs) {
      data_ = rhs.data_;
    }

    operator bool () const {
      return !data_->empty();
    }
  public:
    static inline Source
    FromString(const std::string& source) {
      return Source(Buffer::CopyFrom(source));
    }

    static inline Source
    FromFile(const std::string& filename) {
      auto file = fopen(filename.c_str(), "rb");
      if(file == nullptr) {
        LOG(ERROR) << "failed to open shader source from file: " << filename;
        return Source();
      }
      const auto filesize = GetFilesize(file);
      const auto data = Buffer::New(filesize);
      if(!data->ReadFrom(file)) {
        LOG(ERROR) << "failed to read shader source from file: " << filename;
        return Source();
      }
      return Source(data);
    }
  };
}

#endif//MCC_SHADER_SOURCE_H