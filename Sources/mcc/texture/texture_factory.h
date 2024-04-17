#ifndef MCC_TEXTURE_FACTORY_H
#define MCC_TEXTURE_FACTORY_H

#include "mcc/image/image.h"
#include "mcc/texture/texture_constants.h"

#include "mcc/texture/texture_wrap.h"
#include "mcc/texture/texture_filter.h"

namespace mcc::texture {
  class TextureFactory {
  protected:
    TextureWrap wrap_;
    TextureFilter filter_;

    inline void ApplyProperties() const {
      wrap_.ApplyTo(target());
      filter_.ApplyTo(target());
    }

    virtual void ApplyImage(img::Image* image) const = 0;
  public:
    TextureFactory():
      wrap_(),
      filter_(kLinear, kLinear) {
    }
    virtual ~TextureFactory() = default;
    virtual TextureTarget target() const = 0;

    const TextureWrap& wrap() const {
      return wrap_;
    }

    void SetTextureWrap(const TextureWrap& wrap) {
      wrap_ = wrap;
    }

    const TextureFilter& filter() const {
      return filter_;
    }

    void SetTextureFilter(const TextureFilter& filter) {
      filter_ = filter;
    }

    virtual TextureId Create() const;
    virtual TextureId Create(img::Image* image) const;
  };

  template<const TextureTarget Target>
  class TextureFactoryTemplate : public TextureFactory {
  public:
    TextureFactoryTemplate() = default;
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
    ~TextureFactory2D() override = default;
  };
}

#endif //MCC_TEXTURE_FACTORY_H