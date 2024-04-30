#ifndef MCC_FBO_SCOPE_H
#define MCC_FBO_SCOPE_H

#include "mcc/fbo/fbo_id.h"

namespace mcc::fbo {
  class Fbo;
  class FboScope {
  protected:
    Fbo* fbo_;
  public:
    explicit FboScope(Fbo* fbo):
      fbo_(fbo) {
    }
    virtual ~FboScope() = default;

    Fbo* GetFbo() const {
      return fbo_;
    }

    FboId GetFboId() const;
  };
}

#include "mcc/fbo/fbo_scope_bind.h"

#endif //MCC_FBO_SCOPE_H