#ifndef MCC_TEXTURE_SPEC_H
#define MCC_TEXTURE_SPEC_H

#include "mcc/uri.h"
#include "mcc/texture/texture_wrap.h"
#include "mcc/texture/texture_filter.h"
#include "mcc/texture/texture_alignment.h"

namespace mcc::texture {
  class TextureSpec {
  public:
    virtual ~TextureSpec() = default;
    virtual TextureTarget GetTextureTarget() const = 0;
    virtual TextureWrap GetTextureWrap() const = 0;
    virtual TextureFilter GetTextureFilter() const = 0;
  };
}

#include "mcc/texture/texture_spec_json.h"
#include "mcc/texture/texture_spec_builder.h"

#endif //MCC_TEXTURE_SPEC_H