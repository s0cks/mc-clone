#include "mcc/fbo/fbo_factory.h"

namespace mcc::fbo {
  rx::observable<Fbo*> FboFactory::Create(const int num) const {
    return GenerateFboId(num)
      .filter(IsValidFboId)
      .map([this](FboId id) {
        return CreateFbo(id);
      });
  }

  Fbo* FboFactory::CreateFbo(const FboId id) const {
    NOT_IMPLEMENTED(FATAL);
    return nullptr;
  }
}