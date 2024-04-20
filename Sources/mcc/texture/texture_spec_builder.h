#ifndef MCC_TEXTURE_SPEC_H
#error "Please #include <mcc/texture/texture_spec.h> instead."
#endif //MCC_TEXTURE_SPEC_H

#ifndef MCC_TEXTURE_SPEC_BUILDER_H
#define MCC_TEXTURE_SPEC_BUILDER_H

#include "mcc/texture/texture_spec.h"

namespace mcc::texture {
  template<const TextureTarget Target>
  class TextureSpecBuilder {
    class TextureSpecImpl : public TextureSpec {
    protected:
      TextureWrap wrap_;
      TextureFilter filter_;
    public:
      TextureSpecImpl(const TextureWrap& wrap,  
                      const TextureFilter& filter):
        TextureSpec(),
        wrap_(wrap),
        filter_(filter) {
      }
      ~TextureSpecImpl() override = default;

      TextureWrap GetTextureWrap() const override {
        return wrap_;
      }

      TextureFilter GetTextureFilter() const override {
        return filter_;
      }

      TextureTarget GetTextureTarget() const override {
        return Target;
      }
    };
  private:
    TextureWrap wrap_;
    TextureFilter filter_;
  public:
    TextureSpecBuilder():
      wrap_(),
      filter_(kLinear) {
    }
    virtual ~TextureSpecBuilder() = default;

    inline const TextureWrap& GetTextureWrap() const {
      return wrap_;
    }

    inline void SetTextureWrap(const TextureWrap& wrap) {
      wrap_ = wrap;
    }

    inline const TextureFilter& GetTextureFilter() const {
      return filter_;
    }

    inline void SetTextureFilter(const TextureFilter& filter) {
      filter_ = filter;
    }

    virtual TextureSpec* Build() const {
      return new TextureSpecImpl(wrap_, filter_);
    }

    void operator<<(const TextureWrap& wrap) {
      return SetTextureWrap(wrap);
    }

    void operator<<(const TextureFilter& filter) {
      return SetTextureFilter(filter);
    }
  };
}

#endif //MCC_TEXTURE_SPEC_BUILDER_H