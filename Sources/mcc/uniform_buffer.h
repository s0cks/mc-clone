#ifndef MCC_UNIFORM_BUFFER_H
#define MCC_UNIFORM_BUFFER_H

#include "mcc/buffer_object.h"

namespace mcc {
  class UniformBufferObject : public BufferObjectTemplate<kUniform> {
  public:
    explicit UniformBufferObject() = default;
    UniformBufferObject(const BufferObjectId id):
      BufferObjectTemplate(id) {
    }
    UniformBufferObject(const UniformBufferObject& rhs) = default;
    ~UniformBufferObject() override = default;
    virtual uint64_t length() const = 0;
    virtual uint64_t elem_size() const = 0;

    virtual uint64_t size() const {
      return length() * elem_size();
    }
  };

  template<typename T, const GlObjectUsage Usage = kDefaultUsage>
  class UniformBufferObjectTemplate : public UniformBufferObject {
  public:
    static constexpr const uint64_t kElementSize = sizeof(T);
  protected:
    uint64_t length_;

    UniformBufferObjectTemplate() = default;
    explicit UniformBufferObjectTemplate(const BufferObjectId id):
      UniformBufferObject(id) {
    }
    UniformBufferObjectTemplate(T* values, const uint64_t num_values):
      UniformBufferObject(),
      length_(num_values) {
      BindBufferData(values, num_values);
    }
    explicit UniformBufferObjectTemplate(const uint64_t num_values):
      UniformBufferObject(),
      length_(num_values) {
      BindBufferData(num_values);
    }
  public:
    ~UniformBufferObjectTemplate() override = default;

    void BufferData(const T* values, const uint64_t num_values) {
      DLOG_IF(ERROR, num_values == 0) << "glBufferData called w/ 0 values.";
      glBufferData(target(), num_values * kElementSize, &values[0], Usage);
      CHECK_GL(FATAL);
    }

    void BindBufferData(const T* values, const uint64_t num_values) {
      Bind();
      BufferData(values, num_values);
    }

    void BufferData(const uint64_t size) {
      DLOG_IF(ERROR, size <= 0) << "glBufferData called w/ " << size << " bytes.";
      glBufferData(target(), size * kElementSize, NULL, Usage);
      CHECK_GL(FATAL);
    }

    void BindBufferData(const uint64_t size) {
      Bind();
      BufferData(size);
    }

    GlObjectUsage usage() const override {
      return Usage;
    }

    uint64_t length() const override {
      return length_;
    }

    uint64_t elem_size() const override {
      return kElementSize;
    }

    void operator=(const UniformBufferObjectTemplate<T, Usage>& rhs) {
      BufferObject::operator=((const BufferObject&) rhs);
      length_ = rhs.length_;
    }
  };
}

#endif //MCC_UNIFORM_BUFFER_H