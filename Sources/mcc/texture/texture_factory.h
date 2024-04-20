#ifndef MCC_TEXTURE_FACTORY_H
#define MCC_TEXTURE_FACTORY_H

#include "mcc/image/image.h"
#include "mcc/texture/texture_constants.h"

#include "mcc/texture/texture_id.h"
#include "mcc/texture/texture_spec.h"
#include "mcc/texture/texture_wrap.h"
#include "mcc/texture/texture_filter.h"

namespace mcc::texture {
  class TextureFactory {
  protected:
    TextureWrap wrap_;
    TextureFilter filter_;

    virtual void ApplyProperties() const;
    virtual void ApplyImage(img::Image* image) const = 0;
  public:
    TextureFactory():
      wrap_(),
      filter_(kLinear, kLinear) {
    }
    explicit TextureFactory(TextureSpec* spec):
      wrap_(spec->GetTextureWrap()),
      filter_(spec->GetTextureFilter()) {
    }
    virtual ~TextureFactory() = default;
    virtual TextureTarget target() const = 0;

    const TextureWrap& wrap() const {
      return wrap_;
    }

    void SetWrap(const TextureWrap& wrap) {
      wrap_ = wrap;
    }

    const TextureFilter& filter() const {
      return filter_;
    }

    void SetFilter(const TextureFilter& filter) {
      filter_ = filter;
    }

    void Apply(TextureSpec* spec) {
      MCC_ASSERT(spec);
      SetFilter(spec->GetTextureFilter());
      SetWrap(spec->GetTextureWrap());
    }

    virtual TextureId Create() const;
    virtual TextureId Create(img::Image* image) const;
  };

  template<const TextureTarget Target>
  class TextureFactoryTemplate : public TextureFactory {
  public:
    TextureFactoryTemplate() = default;
    explicit TextureFactoryTemplate(TextureSpec* spec):
      TextureFactory(spec) {
    }
    ~TextureFactoryTemplate() override = default;
    
    TextureTarget target() const override {
      return Target;
    }
  };

  class TextureFactory2D : public TextureFactoryTemplate<k2D> {
  protected:
    void ApplyImage(img::Image* image) const override;
  public:
    TextureFactory2D() = default;
    explicit TextureFactory2D(TextureSpec* spec):
      TextureFactoryTemplate<k2D>(spec) {
    }
    ~TextureFactory2D() override = default;
  };
}

#endif //MCC_TEXTURE_FACTORY_H