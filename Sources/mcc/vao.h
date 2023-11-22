#ifndef MCC_VAO_H
#define MCC_VAO_H

#include "mcc/gfx.h"

namespace mcc {
  typedef GLuint VertexArrayObjectId;
  static constexpr const VertexArrayObjectId kInvalidVertexArrayObject = 0;
  class VertexArrayObject : public gfx::Resource {
  protected:
    VertexArrayObjectId id_;
  public:
    explicit VertexArrayObject(const VertexArrayObjectId id):
      gfx::Resource(),
      id_(id) {
    }
    VertexArrayObject():
      gfx::Resource(),
      id_(kInvalidVertexArrayObject) {
      glGenVertexArrays(1, &id_);
      CHECK_GL(FATAL);
    }
    VertexArrayObject(const VertexArrayObject& rhs):
      gfx::Resource(),
      id_(rhs.id_) {
    }
    ~VertexArrayObject() override = default;

    VertexArrayObjectId id() const {
      return id_;
    }

    void Bind() const override {
      glBindVertexArray(id_);
      CHECK_GL(FATAL);
    }

    void Unbind() const override {
      glBindVertexArray(kInvalidVertexArrayObject);
      CHECK_GL(FATAL);
    }

    void Delete() override {
      glDeleteVertexArrays(1, &id_);
      CHECK_GL(FATAL);
    }

    void operator=(const VertexArrayObject& rhs) {
      id_ = rhs.id_;
    }

    void operator=(const VertexArrayObjectId& rhs) {
      id_ = rhs;
    }

    explicit operator VertexArrayObjectId () const {
      return id_;
    }

    bool operator==(const VertexArrayObject& rhs) const {
      return id_ == rhs.id_;
    }

    bool operator==(const VertexArrayObjectId& rhs) const {
      return id_ == rhs;
    }

    bool operator!=(const VertexArrayObject& rhs) const {
      return id_ != rhs.id_;
    }

    bool operator!=(const VertexArrayObjectId& rhs) const {
      return id_ != rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const VertexArrayObject& rhs) {
      stream << "VertexArrayObject(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  public:
    static inline void
    GenerateBatch(const uint64_t num_vaos, VertexArrayObject** vaos) {
      MCC_ASSERT((*vaos) == nullptr);
      DLOG(INFO) << "generating " << num_vaos << " VertexArrayObjects....";
      VertexArrayObjectId ids[num_vaos];
      glGenVertexArrays(num_vaos, ids);
      CHECK_GL(FATAL);
      (*vaos) = (VertexArrayObject*)malloc(sizeof(VertexArrayObject) * num_vaos);
      for(auto idx = 0; idx < num_vaos; idx++) {
        const auto vao = VertexArrayObject(ids[idx]);
        memcpy((void*) &vaos[idx], (void*) &vao, sizeof(VertexArrayObject));
      }
      DLOG(INFO) << "done.";
    }
  };
  DEFINE_RESOURCE_SCOPE(VertexArrayObject);
}

#endif //MCC_VAO_H