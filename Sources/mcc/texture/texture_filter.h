#ifndef MCC_TEXTURE_FILTER_H
#define MCC_TEXTURE_FILTER_H

namespace mcc::texture {
  enum TextureFilterComponent : GLenum {
    kNearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    kNearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
    kLinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    kLinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
    kLinear = GL_LINEAR,
    kNearest = GL_NEAREST,

    kDefaultMinFilter = kLinearMipmapLinear,
    kDefaultMagFilter = kLinear,
  };


  struct TextureFilter {
    TextureFilterComponent min;
    TextureFilterComponent mag;

    constexpr TextureFilter(const TextureFilterComponent min_filter,
                            const TextureFilterComponent mag_filter):
      min(min_filter),
      mag(mag_filter) {
    }
    constexpr TextureFilter():
      TextureFilter(kDefaultMinFilter, kDefaultMagFilter) {
    }
    explicit constexpr TextureFilter(const TextureFilterComponent component):
      TextureFilter(component, component) {
    }
    TextureFilter(const TextureFilter& rhs) = default;
    ~TextureFilter() = default;
    TextureFilter& operator=(const TextureFilter& rhs) = default;

    void ApplyTo(const TextureTarget target) {
      glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
      CHECK_GL(FATAL);
      glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
      CHECK_GL(FATAL);
    }
  };

  static constexpr const auto kDefaultFilter = TextureFilter();
  static constexpr const auto kLinearFilter = TextureFilter(kLinear);
  static constexpr const auto kNearestFilter = TextureFilter(kNearest);
}

#endif //MCC_TEXTURE_FILTER_H