#ifndef MCC_FBO_FACTORY_H
#define MCC_FBO_FACTORY_H

#include "mcc/fbo/fbo.h"

namespace mcc::fbo {
  class FboFactory {
  protected:
    Fbo* CreateFbo(const FboId id) const;
  public:
    FboFactory() = default;
    virtual ~FboFactory() = default;
    virtual rx::observable<Fbo*> Create(const int num = 1) const;
  };
}

#endif //MCC_FBO_FACTORY_H