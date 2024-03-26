#ifndef MCC_TEXTURE_LOADER_H
#define MCC_TEXTURE_LOADER_H

#include "mcc/texture/image/image.h"
#include "mcc/texture/texture.h"

namespace mcc::texture {
  class TextureLoader {
  protected:
    TextureLoader() = default;
  public:
    virtual ~TextureLoader() = default;
    virtual std::optional<Texture*> Load(const uri::Uri& uri);
  };
}

#endif //MCC_TEXTURE_LOADER_H