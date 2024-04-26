#ifndef MCC_IBO_SCOPE_H
#define MCC_IBO_SCOPE_H

#include "mcc/ibo/ibo.h"

namespace mcc::ibo {
  class IboScope {
  protected:
    const Ibo* ibo_;
  public:
    explicit IboScope(const Ibo* ibo):
      ibo_(ibo) {
      MCC_ASSERT(ibo);
      Bind();
    }
    virtual ~IboScope() {
      Unbind();
    }

    const Ibo* GetIbo() const {
      return ibo_;
    }

    IboId GetIboId() const {
      return GetIbo()->GetId();
    }

    void Bind() {
      return Ibo::BindIbo(GetIboId());
    }
    
    void Unbind() {
      return Ibo::BindDefaultIbo();
    }
  };

  class IboUpdateScope : public IboScope {
  public:
    explicit IboUpdateScope(const Ibo* ibo):
      IboScope(ibo) {
    }
    ~IboUpdateScope() override = default;

    void Update(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes) {
      Ibo::UpdateBufferData(offset, bytes, num_bytes);
    }

    void Update(const uint8_t* bytes, const uint64_t num_bytes) {
      return Update(0, bytes, num_bytes);
    }
  };

  class IboDrawScope : public IboScope {
  public:
    explicit IboDrawScope(const Ibo* ibo):
      IboScope(ibo) {  
    }
    ~IboDrawScope() override = default;

    void Draw(const Ibo::DrawMode mode, const uint64_t count, const GLvoid* indices = 0) {
      glDrawElements(mode, count, GetIbo()->GetType(), indices);
      CHECK_GL(FATAL);
    }

    void Draw(const Ibo::DrawMode mode) {
      return Draw(mode, GetIbo()->GetLength());
    }
  };
}

#endif //MCC_IBO_SCOPE_H