#ifndef MCC_TEXTURE_BUILDER_H
#define MCC_TEXTURE_BUILDER_H

#include "mcc/texture/texture_wrap.h"
#include "mcc/texture/texture_target.h"
#include "mcc/texture/texture_filter.h"
#include "mcc/texture/texture_alignment.h"

namespace mcc::texture {
  class TextureBuilder {
  public:
    static constexpr const bool kDefaultMipmapValue = false;
    static constexpr const GLenum kDefaultInternalFormatValue = GL_RGBA;
    static constexpr const GLenum kDefaultFormatValue = GL_RGBA;
    static constexpr const GLenum kDefaultTypeValue = GL_UNSIGNED_BYTE;
  protected:
    TextureTarget target_;
    TextureFilter filter_;
    TextureWrap wrap_;
    PixelStoreAlignment alignment_;
    GLenum internal_format_;
    GLenum format_;
    GLenum type_;
    bool mipmap_;
    TextureSize size_;
  public:
    explicit TextureBuilder(const TextureTarget target = k2D):
      target_(target),
      filter_(),
      wrap_(),
      alignment_(),
      internal_format_(kDefaultInternalFormatValue),
      format_(kDefaultFormatValue),
      type_(kDefaultTypeValue),
      mipmap_(kDefaultMipmapValue),
      size_() {
    }
    virtual ~TextureBuilder() = default;

    TextureBuilder& SetMipmap(const bool rhs) {
      mipmap_ = rhs;
      return *this;
    }

    TextureBuilder& SetInternalFormat(const GLenum rhs) {
      internal_format_ = rhs;
      return *this;
    }

    TextureBuilder& SetFormat(const GLenum rhs) {
      format_ = rhs;
      return *this;
    }

    TextureBuilder& SetType(const GLenum rhs) {
      type_ = rhs;
      return *this;
    }

    TextureBuilder& SetSize(const TextureSize rhs) {
      size_ = rhs;
      return *this;
    }

    TextureBuilder& SetTarget(const TextureTarget rhs) {
      target_ = rhs;
      return *this;
    }

    TextureBuilder& SetFilter(const TextureFilter& rhs) {
      filter_ = rhs;
      return *this;
    }

    TextureBuilder& SetAlignment(const PixelStoreAlignment& rhs) {
      alignment_ = rhs;
      return *this;
    }

    TextureBuilder& SetWrap(const TextureWrap& rhs) {
      wrap_ = rhs;
      return *this;
    }
  };
}

#endif //MCC_TEXTURE_BUILDER_H