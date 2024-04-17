#ifndef MCC_TEXTURE_H
#define MCC_TEXTURE_H

#include "mcc/gfx.h"
#include "mcc/uri.h"
#include "mcc/platform.h"
#include "mcc/resource/resource.h"

#include "mcc/texture/texture_options.h"
#include "mcc/texture/texture_constants.h"

namespace mcc::texture {
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

  class TextureData {
  private:
    GLint level_;
    GLint internal_fmt_;
    TextureSize size_;
    GLint border_;
    GLenum fmt_;
    GLenum type_;

    uint8_t* bytes_;
    uint64_t num_bytes_;

    virtual void ApplyTo(const TextureTarget target) const {
      MCC_ASSERT(border() == 0);
      MCC_ASSERT(HasData());
      glTexImage2D(target, level(), internal_format(), width(), height(), border(), format(), type(), data());
      CHECK_GL(FATAL);
    }
  public:
    TextureData() = default;
    TextureData(const TextureData& rhs):
      level_(rhs.level_),
      internal_fmt_(rhs.internal_fmt_),
      size_(rhs.size_),
      border_(rhs.border_),
      fmt_(rhs.fmt_),
      type_(rhs.type_),
      bytes_(rhs.bytes_),
      num_bytes_(rhs.num_bytes_) {
    }
    virtual ~TextureData() = default;

    GLint level() const {
      return level_;
    }

    GLint internal_format() const {
      return internal_fmt_;
    }

    GLint border() const {
      return border_;
    }

    GLenum format() const {
      return fmt_;
    }

    GLenum type() const {
      return type_;
    }

    const TextureSize size() const {
      return size_;
    }

    const uint32_t width() const {
      return size_[0];
    }

    const uint32_t height() const {
      return size_[1];
    }

    const uint8_t* bytes() const {
      return bytes_;
    }

    const uint8_t* bytes_begin() const {
      return bytes();
    }

    const uint8_t* bytes_end() const {
      return bytes() + num_bytes();
    }

    uint64_t num_bytes() const {
      return num_bytes_;
    }

    const GLvoid* data() const {
      return (const GLvoid*) bytes_;
    }

    bool HasData() const {
      return bytes() && num_bytes() >= 1;
    }
  };

  class Texture : public gfx::Resource,
                  public res::ResourceTemplate<res::kTextureType>  {
  public:
    static rx::observable<TextureId> GenerateTextureId(const int num = 1);
  protected:
    TextureId id_;

    void Destroy () override;
  public:
    Texture() = default;
    explicit Texture(const TextureId id = kInvalidTextureId):
      gfx::Resource(),
      res::ResourceTemplate<res::kTextureType>(),
      id_(id) {
    }
    Texture(const Texture& rhs):
      gfx::Resource(),
      res::ResourceTemplate<res::kTextureType>(),
      id_(rhs.id_) {
    }
    ~Texture() override = default;
    virtual TextureTarget GetTextureTarget() const = 0;

    TextureId id() const {
      return id_;
    }

    TextureWrap GetTextureWrap() const;

    Pixel GetPixel(const uint32_t x, const uint32_t y) {
      // glReadBuffer(GetTextureTarget());
      // CHECK_GL(FATAL);
      // Pixel pixel;
      // glReadPixels(x, y, 1, 1, data_.format(), data_.type(), &pixel);
      // CHECK_GL(FATAL);
      // return pixel;
      return {};
    }

    Pixel GetPixel(const TextureCoord& coord) {
      return GetPixel(coord[0], coord[1]);
    }

    void Bind() const override {
      glBindTexture(GetTextureTarget(), id_);
      CHECK_GL(FATAL);
    }

    void Unbind() const override {
      glBindTexture(GetTextureTarget(), kInvalidTextureId);
      CHECK_GL(FATAL);
    }

    virtual void Bind(const uint32_t slot) const {
      glActiveTexture(GL_TEXTURE0 + slot);
      CHECK_GL(FATAL);
      Bind();
    }

    inline void Bind0() const {
      return Bind(0);
    }

    inline void Bind1() const {
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
  }

  using resource::TextureRef;

  static inline TextureRef
  NullTexture() {
    return TextureRef();
  }

  TextureRef GetTexture(const uri::Uri& uri);

  static inline TextureRef
  GetTexture(const uri::basic_uri& uri) {
    return GetTexture(uri::Uri(uri));
  }

  template<const uint32_t Slot, const texture::TextureTarget Target = texture::k2D>
  class TextureBindScope { //TODO: move to mcc::texture
  private:
    texture::TextureId id_; //TODO: need to hold reference to this texture

    inline void Bind() const {
      glBindTexture(Target, id_);
      CHECK_GL(FATAL);
      glActiveTexture(GL_TEXTURE0 + Slot);
      CHECK_GL(FATAL);
    }

    void Unbind() const {
      glBindTexture(Target, kInvalidTextureId);
      CHECK_GL(FATAL);
    }
  public:
    explicit TextureBindScope(const texture::TextureId id):
      id_(id) {
      Bind();
    }
    explicit TextureBindScope(const texture::Texture* texture):
      TextureBindScope(texture->id()) {
    }
    explicit TextureBindScope(const TextureRef& ref):
      TextureBindScope(ref->id()) {
    }
    ~TextureBindScope() {
      Unbind();
    }

    texture::TextureId id() const {
      return id_;
    }

    uint32_t slot() const {
      return Slot;
    }
  };
}

#endif //MCC_TEXTURE_H