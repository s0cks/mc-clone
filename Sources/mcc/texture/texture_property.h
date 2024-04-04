#ifndef MCC_TEXTURE_PROPERTY_H
#define MCC_TEXTURE_PROPERTY_H

#include "mcc/texture/texture_target.h"
#include "mcc/texture/texture_constants.h"

namespace mcc::texture {
  class TextureProperty {
  protected:
    virtual void ApplyTo(const TextureTarget id) = 0;
  public:
    TextureProperty() = default;
    virtual ~TextureProperty() = default;
    virtual const char* GetName() const = 0;
  };
}

#endif //MCC_TEXTURE_PROPERTY_H