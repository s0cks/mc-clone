#ifndef MCC_TEXTURE_H
#define MCC_TEXTURE_H

#include <optional>
#include "mcc/gfx.h"
#include "mcc/uri.h"
#include "mcc/json.h"
#include "mcc/platform.h"
#include "mcc/resource/resource.h"

#include "mcc/texture/texture_id.h"
#include "mcc/texture/texture_events.h"
#include "mcc/texture/texture_loader.h"
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

  namespace json {
    template<const bool Const>
    class TextureObject {
    public:
      static constexpr const auto kFilePropertyName = "file"; //TODO: move to tex1d,tex2d,tex3d
      static constexpr const auto kFilterPropertyName = "filter";
      static constexpr const auto kWrapPropertyName = "wrap";
    private:
      typedef GenericObject<Const, Document::ValueType> ObjectType;
    protected:
      const ObjectType& value_;
    
      explicit TextureObject(const ObjectType& value):
        value_(value) {
      }

      inline const ObjectType& value() const {
        return value_;
      }
    protected:
      inline std::optional<const Value*>
      GetProperty(const char* name) const {
        if(!value().HasMember(name))
          return std::nullopt;
        return { &value()[name] };
      }
    public:
      virtual ~TextureObject() = default;

      virtual std::optional<const Value*> GetFileProperty() const {
        return GetProperty(kFilePropertyName);
      }

      virtual std::optional<const Value*> GetFilterProperty() const {
        return GetProperty(kFilterPropertyName);
      }

      virtual std::optional<const Value*> GetWrapProperty() const {
        return GetProperty(kWrapPropertyName);
      }
    };
  }

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
      friend class TextureBindScope;
      friend class TextureBuilder;
      friend class TextureBuilderBase;
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

      static void BindTexture(const TextureTarget target, const TextureId id);
      static void DeleteTextures(const TextureId* ids, const uint64_t num_ids);
      static void ActiveTexture(const int32_t slot);

      static inline void
      DeleteTexture(const TextureId id) {
        return DeleteTextures(&id, 1);
      }
      
      static inline void
      UnbindTexture(const TextureTarget target) {
        return BindTexture(target, kInvalidTextureId);
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
      virtual std::string ToString() const;

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
  }
}

#include "mcc/texture/texture_2d.h"
#include "mcc/texture/texture_3d.h"
#include "mcc/texture/cube_map.h"

#endif //MCC_TEXTURE_H