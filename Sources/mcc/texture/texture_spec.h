#ifndef MCC_TEXTURE_SPEC_H
#define MCC_TEXTURE_SPEC_H

#include <optional>

#include "mcc/uri.h"
#include "mcc/spec.h"
#include "mcc/json.h"
#include "mcc/common.h"

#include "mcc/texture/texture_constants.h"
#include "mcc/texture/texture_wrap.h"
#include "mcc/texture/texture_filter.h"
#include "mcc/texture/texture_alignment.h"

namespace mcc::texture {
  class TextureSpec : public Spec {
    DEFINE_NON_COPYABLE_TYPE(TextureSpec);
  protected:
    TextureSpec() = default;
  public:
    ~TextureSpec() override = default;
    DEFINE_SPEC(Texture);
    DECLARE_SPEC_PROPERTY(File, uri::Uri);
    DECLARE_SPEC_PROPERTY(Target, TextureTarget);
    DECLARE_SPEC_PROPERTY(Wrap, TextureWrap);
    DECLARE_SPEC_PROPERTY(Filter, TextureFilter);
    DECLARE_SPEC_PROPERTY(Alignment, PixelStoreAlignment);

    friend std::ostream& operator<<(std::ostream& stream, const TextureSpec& rhs) {
      stream << "TextureSpec(";
      stream << ")";
      return stream;
    }
  public:
    static rx::observable<TextureSpec*> New(const uri::Uri& file,
                                            const TextureTarget& target = k2D,
                                            const TextureWrap& wrap = kDefaultWrap,
                                            const TextureFilter& filter = kDefaultFilter,
                                            const PixelStoreAlignment& alignment = kDefaultAlignment);
    static rx::observable<TextureSpec*> FromJson(const uri::Uri& file);
  };
}

#endif //MCC_TEXTURE_SPEC_H