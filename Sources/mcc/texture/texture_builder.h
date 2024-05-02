#ifndef MCC_TEXTURE_BUILDER_H
#define MCC_TEXTURE_BUILDER_H

#include "mcc/texture/texture.h"
#include "mcc/texture/texture_format.h"

namespace mcc::texture {
  class TextureBuilder {
  protected:
    TextureTarget target_;
    TextureFormat internal_format_;
    TextureFormat format_;
    TextureFilter filter_;
    TextureWrap wrap_;
    TextureSize size_;
    int level_;
    int border_;
    GLenum type_;

    Texture* InitTexture(const TextureId id) const;
  public:
    explicit TextureBuilder(const TextureTarget target):
      target_(target),
      format_(kDefaultTextureFormat),
      internal_format_(kDefaultTextureFormat),
      filter_(kLinearFilter),
      wrap_(kClampToEdgeWrap),
      size_(0),
      level_(0),
      border_(0),
      type_(GL_UNSIGNED_BYTE) {
    }
    virtual ~TextureBuilder() = default;

    virtual void SetTarget(const TextureTarget target) {
      target_ = target;
    }

    virtual void SetFormat(const TextureFormat format) {
      format_ = format;
    }

    virtual void SetInternalFormat(const TextureFormat format) {
      internal_format_ = format;
    }

    virtual void SetSize(const TextureSize& size) {
      size_ = size;
    }

    virtual void SetWrap(const TextureWrap& wrap) {
      wrap_ = wrap;
    }

    virtual void SetFilter(const TextureFilter& filter) {
      filter_ = filter;
    }

    virtual void SetLevel(const int level) {
      level_ = level;
    }

    virtual void SetBorder(const int border) {
      border_ = border;
    }

    virtual void SetType(const GLenum type) {
      type_ = type;
    }

    virtual rx::observable<Texture*> Build(const int num = 1) const;
  };
}

#endif //MCC_TEXTURE_BUILDER_H