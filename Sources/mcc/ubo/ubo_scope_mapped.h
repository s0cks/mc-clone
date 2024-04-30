#ifndef MCC_UBO_SCOPE_H
#error "Please #include <mcc/ubo/ubo_scope.h> instead."
#endif //MCC_UBO_SCOPE_H

#ifndef MCC_UBO_SCOPE_MAPPED_H
#define MCC_UBO_SCOPE_MAPPED_H

#include "mcc/ubo/ubo_scope.h"

namespace mcc::ubo {
  class UboMappedScope : public UboBindScope {
  public:
    enum Access : GLenum {
      kNone,
      kReadOnly = GL_READ_ONLY,
      kWriteOnly = GL_WRITE_ONLY,
    };
  protected:
    Access access_;
    uword start_;
  public:
    explicit UboMappedScope(Ubo* ubo, const Access access);
    ~UboMappedScope() override;

    uint64_t GetSize() const;

    uword GetStartingAddress() const {
      return start_;
    }

    void* GetStartingAddressPointer() const {
      return (void*) GetStartingAddress();
    }

    uword GetEndingAddress() const {
      return GetStartingAddress() + GetSize();
    }

    void* GetEndingAddressPointer() const {
      return (void*) GetEndingAddress();
    }

    bool IsMapped() const {
      return GetStartingAddress() != 0;
    }

    Access GetAccess() const {
      return access_;
    }

    bool IsReadOnly() const {
      return GetAccess() == kReadOnly;
    }

    bool IsWriteOnly() const {
      return GetAccess() == kWriteOnly;
    }
  };
}

#endif //MCC_UBO_SCOPE_MAPPED_H