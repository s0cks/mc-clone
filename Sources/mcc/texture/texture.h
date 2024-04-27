#ifndef MCC_TEXTURE_H
#define MCC_TEXTURE_H

#include "mcc/gfx.h"
#include "mcc/uri.h"
#include "mcc/platform.h"
#include "mcc/resource/resource.h"

#include "mcc/texture/texture_id.h"
#include "mcc/texture/texture_events.h"
#include "mcc/texture/texture_options.h"
#include "mcc/texture/texture_constants.h"

namespace mcc {
  namespace texture {
    class Texture;
  }
  using texture::Texture;

  namespace resource {
    typedef Reference<Texture> TextureRef;
  }
  using res::TextureRef;

  namespace texture {
    rx::observable<TextureEvent*> OnTextureEvent();

    static inline rx::observable<TextureEvent*>
    OnTextureEvent(const TextureId id) {
      return OnTextureEvent()
        .filter([id](TextureEvent* event) {
          return event
              && event->GetTextureId() == id;
        });
    }

#define DEFINE_ON_TEXTURE_EVENT(Name)                       \
    static inline rx::observable<Name##Event*>              \
    On##Name##Event() {                                     \
      return OnTextureEvent()                               \
        .filter(Name##Event::Filter)                        \
        .map(Name##Event::Cast);                            \
    }                                                       \
    static inline rx::observable<Name##Event*>              \
    On##Name##Event(const TextureId id) {                   \
      return OnTextureEvent()                               \
        .filter(Name##Event::FilterBy(id))                  \
        .map(Name##Event::Cast);                            \
    }
  //TODO:
#undef DEFINE_ON_TEXTURE_EVENT

    class Texture : public gfx::Resource,
                    public res::ResourceTemplate<res::kTextureType>  {
    public:
      static rx::observable<TextureId> GenerateTextureId(const int num = 1);
    protected:
      TextureId id_;

      void Destroy () override;

      static void Publish(TextureEvent* event);

      template<class E, typename... Args>
      void Publish(Args... args) const {
        E event(GetTextureId(), args...);
        return Publish(&event);
      }
    public:
      Texture() = default;
      explicit Texture(const TextureId id):
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

      TextureId GetTextureId() const {
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
    };

    template<const TextureTarget Target>
    class TextureTemplate : public Texture {
    protected:
      explicit TextureTemplate(const TextureId id):
        Texture(id) {
      }
    public:
      ~TextureTemplate() override = default;

      TextureTarget GetTextureTarget() const override {
        return Target;
      }
    };

    class Texture2D : public TextureTemplate<k2D> {
    public:
      explicit Texture2D(const TextureId id):
        TextureTemplate<k2D>(id) {
      }
      ~Texture2D() override = default;
    public:
      static inline TextureRef
      New(const TextureId id) {
        return TextureRef(new Texture2D(id));
      }

      static TextureRef New(const uri::Uri& uri);
      
      static inline TextureRef
      New(const uri::basic_uri& uri) {
        auto target = uri;
        if(!StartsWith(target, "texture"))
          target = fmt::format("texture://{0:s}", uri);
        return New(uri::Uri(target));
      }
    };

    class Texture3D : public TextureTemplate<k3D> {
    protected:
      explicit Texture3D(const TextureId id):
        TextureTemplate<k3D>(id) {
      }
    public:
      ~Texture3D() override = default;
    public:
      static TextureRef New(const uri::Uri& uri);
      
      static inline TextureRef
      New(const uri::basic_uri& uri) {
        return New(uri::Uri(uri));
      }
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

    class CubeMap : public TextureTemplate<kCubeMap> {
    public:
      static TextureRef New(const uri::Uri& uri);
      
      static inline TextureRef
      New(const uri::basic_uri& uri) {
        return New(uri::Uri(uri));
      }
    };

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
        TextureBindScope(texture->GetTextureId()) {
      }
      explicit TextureBindScope(const TextureRef& ref):
        TextureBindScope(ref->GetTextureId()) {
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
  using texture::Texture2D;
  using texture::Texture3D;
  using texture::CubeMap;
  using texture::TextureBindScope;
}

#endif //MCC_TEXTURE_H