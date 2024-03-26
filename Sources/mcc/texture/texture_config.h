#ifndef MCC_TEXTURE_CONFIG_H
#define MCC_TEXTURE_CONFIG_H

#include "mcc/texture/texture_target.h"
#include "mcc/texture/texture_filter.h"
#include "mcc/texture/texture_alignment.h"
#include "mcc/texture/texture_wrap.h"

namespace mcc::texture {
  struct TextureConfig {
    TextureTarget target;
    GLenum fmt_internal;
    TextureSize size;
    GLenum fmt;
    GLenum type;
    bool mipmap;
    PixelStoreAlignment alignment;
    TextureFilter filter;
    TextureWrap wrap;

    TextureConfig(TextureTarget Target,
                  GLenum InternalFormat,
                  TextureSize Size,
                  GLenum Format,
                  GLenum Type,
                  bool Mipmap,
                  PixelStoreAlignment Alignment,
                  TextureFilter Filter,
                  TextureWrap Wrap):
      target(Target),
      fmt_internal(InternalFormat),
      size(Size),
      format(Format),
      type(Type),
      mipmap(Mipmap),
      alignment(Alignment),
      filter(Filter),
      wrap(Wrap) {
    }
    ~TextureConfig() = default;
  };
}

#endif //MCC_TEXTURE_CONFIG_H