#ifndef MCC_TEXTURE_H
#define MCC_TEXTURE_H

#include "mcc/gfx.h"
#include "mcc/platform.h"
#include "mcc/resource.h"

namespace mcc::texture {
  typedef glm::u64vec2 TextureSize;

  typedef GLuint TextureId;

  static constexpr const TextureId kInvalidTextureId = 0;

  enum TextureTarget : GLenum {
    k1D = GL_TEXTURE_1D,
    k2D = GL_TEXTURE_2D,
    k3D = GL_TEXTURE_3D,
    kCubeMap = GL_TEXTURE_CUBE_MAP,

    kDefaultTarget = k2D,
  };

  static inline std::ostream& operator<<(std::ostream& stream, const TextureTarget& rhs) {
    switch(rhs) {
      case k1D: return stream << "1D";
      case k2D: return stream << "2D";
      case k3D: return stream << "3D";
      case kCubeMap: return stream << "CubeMap";
      default:
        return stream << "Unknown";
    }
  }


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

  enum TextureAlignment : GLint {
    kNone = 0,
    k1 = 1,
    k2 = 2,
    k4 = 4,
    k8 = 8,

    kByteAlignment = k1,
    kRowAlignment = k2,
    kWordAlignment = k4,
    kDoubleWordAlignment = k8,


    kDefaultPackAlignment = k4,
    kDefaultUnpackAlignment = k4,
  };

  enum CubeMapFace {
    kRightFace  = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    kLeftFace   = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    kTopFace    = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    kBottomFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    kBackFace   = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    kFrontFace  = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    kNumberOfCubeMapFaces,
  };

  static inline const char*
  ToString(const CubeMapFace& rhs) {
    switch(rhs) {
      case kRightFace:
        return "right";
      case kLeftFace:
        return "left";
      case kTopFace:
        return "top";
      case kBottomFace:
        return "bottom";
      case kBackFace:
        return "back";
      case kFrontFace:
        return "front";
      default:
        return "Unknown";
    }
  }

  static inline std::ostream& operator<<(std::ostream& stream, const CubeMapFace& rhs) {
    switch(rhs) {
      case kRightFace: return stream << "Right";
      case kLeftFace: return stream << "Left";
      case kTopFace: return stream << "Top";
      case kBottomFace: return stream << "Bottom";
      case kBackFace: return stream << "Back";
      case kFrontFace: return stream << "Front";
      default: return stream << "Unknown";
    }
  }

  struct PixelStoreAlignment {
    TextureAlignment pack;
    TextureAlignment unpack;

    PixelStoreAlignment(const TextureAlignment pack_alignment,
                        const TextureAlignment unpack_alignment):
                        pack(pack_alignment),
                        unpack(unpack_alignment) {
    }
    PixelStoreAlignment():
      PixelStoreAlignment(kDefaultPackAlignment, kDefaultUnpackAlignment) {
    }
    PixelStoreAlignment(const PixelStoreAlignment& rhs) = default;
    ~PixelStoreAlignment() = default;

    PixelStoreAlignment& operator=(const PixelStoreAlignment& rhs) = default;

    void Apply() {
      glPixelStorei(GL_PACK_ALIGNMENT, pack);
      CHECK_GL(FATAL);
      glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
      CHECK_GL(FATAL);
    }
  };

  struct TextureFilter {
    TextureFilterComponent min;
    TextureFilterComponent mag;

    TextureFilter(const TextureFilterComponent min_filter,
                  const TextureFilterComponent mag_filter):
                  min(min_filter),
                  mag(mag_filter) {
    }
    TextureFilter():
      TextureFilter(kDefaultMinFilter, kDefaultMagFilter) {
    }
    explicit TextureFilter(const TextureFilterComponent component):
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

  enum TextureWrapMode : GLenum {
    kRepeat = GL_REPEAT,
    kClampToEdge = GL_CLAMP_TO_EDGE,
    kClampToBorder = GL_CLAMP_TO_BORDER,

    kDefaultWrapMode = kClampToEdge,
  };

  struct TextureWrap {
    TextureWrapMode r;
    TextureWrapMode s;
    TextureWrapMode t;

    TextureWrap(const TextureWrapMode R,
                const TextureWrapMode S,
                const TextureWrapMode T):
      r(R),
      s(S),
      t(T) {
    }
    TextureWrap(const TextureWrapMode mode):
      TextureWrap(mode, mode, mode) {
    }
    TextureWrap():
      TextureWrap(kDefaultWrapMode) {
    }
    TextureWrap(const TextureWrap& rhs) = default;
    ~TextureWrap() = default;
    TextureWrap& operator=(const TextureWrap& rhs) = default;

    void ApplyTo(const TextureTarget target) {
      glTexParameteri(target, GL_TEXTURE_WRAP_R, r);
      CHECK_GL(FATAL);
      glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
      CHECK_GL(FATAL);
      glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
      CHECK_GL(FATAL);
    }
  };

  class Texture : public gfx::Resource {
  protected:
    TextureId id_;
    TextureTarget target_;
    GLenum internal_format_;
    TextureSize size_;
    GLenum format_;
    GLenum type_;
    bool mipmap_;
    PixelStoreAlignment alignment_;
    TextureFilter filter_;
    TextureWrap wrap_;
  private:
    void InitializeTexture(const bool generate,
                           const bool bind,
                           const bool unbind,
                           const GLvoid* pixels) {
      if(generate) {
        glGenTextures(1, &id_);
        CHECK_GL(FATAL);
        if(bind) {
          glBindTexture(target_, id_);
          CHECK_GL(FATAL);
          alignment_.Apply();
          if(pixels) {
            DLOG(INFO) << "loading pixels for texture....";
            glTexImage2D(target_, 0, internal_format_, size_[0], size_[1], 0, format_, type_, pixels);
            CHECK_GL(FATAL);
          }
          if(mipmap_) {
            DLOG(INFO) << "generating mipmap for texture....";
            glGenerateMipmap(target_);
            CHECK_GL(FATAL);
          }
          filter_.ApplyTo(target_);
          wrap_.ApplyTo(target_);
          if(unbind) {
            glBindTexture(target_, 0);
            CHECK_GL(FATAL);
          }
        }
      }
    }
  public:
    explicit Texture(const TextureTarget target,
                     const bool generate = true,
                     const bool bind = true,
                     const bool unbind = true,
                     const PixelStoreAlignment& alignment = {},
                     const TextureFilter& filter = {},
                     const TextureWrap& wrap = {}):
      Resource(),
      id_(kInvalidTextureId),
      target_(target),
      internal_format_(),
      size_(),
      format_(),
      type_(),
      mipmap_(false),
      alignment_(alignment),
      filter_(filter),
      wrap_(wrap) {
      if(generate)
        InitializeTexture(generate, bind, unbind, NULL);
    }
    explicit Texture(const TextureTarget target,
                     const bool generate,
                     const bool bind,
                     const bool unbind,
                     const GLenum internal_format,
                     const TextureSize& size,
                     const GLenum format,
                     const GLenum type,
                     const bool mipmap = false,
                     const TextureAlignment pack_align = kDefaultPackAlignment,
                     const TextureAlignment unpack_align = kDefaultUnpackAlignment,
                     const TextureFilterComponent min = kDefaultMinFilter,
                     const TextureFilterComponent mag = kDefaultMagFilter,
                     const TextureWrapMode wrapR = kDefaultWrapMode,
                     const TextureWrapMode wrapS = kDefaultWrapMode,
                     const TextureWrapMode wrapT = kDefaultWrapMode,
                     const GLvoid* pixels = NULL):
      Resource(),
      id_(kInvalidTextureId),
      target_(target),
      internal_format_(internal_format),
      size_(size),
      format_(format),
      type_(type),
      mipmap_(mipmap),
      alignment_(pack_align, unpack_align),
      filter_(min, mag),
      wrap_(wrapR, wrapS, wrapT) {
      InitializeTexture(generate, bind, unbind, pixels);
    }
    explicit Texture(const TextureTarget target,
                     const bool generate,
                     const bool bind,
                     const bool unbind,
                     const GLenum internal_format,
                     const TextureSize& size,
                     const GLenum format,
                     const GLenum type,
                     const bool mipmap,
                     const PixelStoreAlignment& alignment,
                     const TextureFilter& filter,
                     const TextureWrap& wrap,
                     const GLvoid* pixels = NULL):
      Resource(),
      id_(kInvalidTextureId),
      target_(target),
      internal_format_(internal_format),
      size_(size),
      format_(format),
      type_(type),
      mipmap_(mipmap),
      alignment_(alignment),
      filter_(filter),
      wrap_(wrap) {
      InitializeTexture(generate, bind, unbind, pixels);
    }
    explicit Texture(const TextureTarget target,
                     const bool generate,
                     const bool bind,
                     const bool unbind,
                     const GLenum internal_format,
                     const TextureSize& size,
                     const GLenum format,
                     const GLenum type,
                     const bool mipmap = false,
                     const GLvoid* pixels = NULL):
      Resource(),
      id_(kInvalidTextureId),
      target_(target),
      internal_format_(internal_format),
      size_(size),
      format_(format),
      type_(type),
      mipmap_(mipmap),
      alignment_(),
      filter_(),
      wrap_() {
      InitializeTexture(generate, bind, unbind, pixels);
    }
    Texture(const Texture& rhs):
      Resource(),
      id_(rhs.id_),
      size_(rhs.size_) {
    }
    ~Texture() override = default;

    TextureId id() const {
      return id_;
    }

    TextureTarget target() const {
      return target_;
    }

    GLenum internal_format() const {
      return internal_format_;
    }

    TextureSize size() const {
      return size_;
    }

    GLenum format() const {
      return format_;
    }

    GLenum type() const {
      return type_;
    }

    PixelStoreAlignment alignment() const {
      return alignment_;
    }

    bool mipmap() const {
      return mipmap_;
    }

    TextureFilter filter() const {
      return filter_;
    }

    TextureWrap wrap() const {
      return wrap_;
    }

    void Bind() const override {
      glBindTexture(target_, id_);
      CHECK_GL(FATAL);
    }

    void Unbind() const override {
      glBindTexture(target_, kInvalidTextureId);
      CHECK_GL(FATAL);
    }

    void Delete() override {
      glDeleteTextures(1, &id_);
      CHECK_GL(FATAL);
    }

    void Bind(const uint32_t slot) const {
      glActiveTexture(GL_TEXTURE0 + slot);
      CHECK_GL(FATAL);
      Bind();
    }

    void Bind0() const {
      return Bind(0);
    }

    void Bind1() const {
      return Bind(1);
    }

    constexpr operator TextureId () const {
      return id_;
    }

    void operator=(const Texture& rhs) {
      id_ = rhs.id_;
    }

    void operator=(const TextureId& rhs) {
      id_ = rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Texture& rhs) {
      stream << "Texture(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };
}

namespace mcc {
  using texture::Texture;
  using texture::kInvalidTextureId;

  namespace resource {
    typedef Reference<Texture> TextureRef;

    static inline Tag
    NewTextureTag(const std::string& name) {
      return Tag::Texture(name);
    }
  }

  using resource::TextureRef;

  static inline TextureRef
  NullTexture() {
    return TextureRef();
  }

  TextureRef GetTexture(const resource::Token& token);

  static inline TextureRef
  GetTextureFromFile(const std::string& location) {
    return GetTexture(res::Token(res::Tag::Texture(location), location));
  }

  static inline TextureRef
  GetTexture(const std::string& name) {
    const auto token = resource::Registry::Get(resource::NewTextureTag(name));
    return GetTexture(token);
  }
}

#endif //MCC_TEXTURE_H