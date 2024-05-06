#ifndef MCC_TEXTURE_H
#error "Please #include <mcc/texture/texture.h> instead."
#endif //MCC_TEXTURE_H

#ifndef MCC_CUBE_MAP_H
#define MCC_CUBE_MAP_H

#include "mcc/texture/texture.h"
#include "mcc/texture/cube_map_face.h"
#include "mcc/texture/cube_map_builder.h"

namespace mcc {
  namespace json {
    template<const bool Const>
    class GenericCubeMapObject : public TextureObject<Const> {
    public:
      static constexpr const auto kFrontPropertyName = "front";
      static constexpr const auto kBackPropertyName = "back";
      static constexpr const auto kLeftPropertyName = "left";
      static constexpr const auto kRightPropertyName = "right";
      static constexpr const auto kTopPropertyName = "top";
      static constexpr const auto kBottomPropertyName = "bottom";
    private:
      typedef GenericObject<Const, Document::ValueType> ObjectType;
    public:
      explicit GenericCubeMapObject(const ObjectType& value):
        TextureObject<Const>(value) {
      }
      ~GenericCubeMapObject() override = default;

      virtual std::optional<const Value*> GetFrontProperty() const {
        return TextureObject<Const>::GetProperty(kFrontPropertyName);
      }

      virtual std::optional<const Value*> GetBackProperty() const {
        return TextureObject<Const>::GetProperty(kBackPropertyName);
      }

      virtual std::optional<const Value*> GetTopProperty() const {
        return TextureObject<Const>::GetProperty(kTopPropertyName);
      }

      virtual std::optional<const Value*> GetBottomProperty() const {
        return TextureObject<Const>::GetProperty(kBottomPropertyName);
      }

      virtual std::optional<const Value*> GetLeftProperty() const {
        return TextureObject<Const>::GetProperty(kLeftPropertyName);
      }

      virtual std::optional<const Value*> GetRightProperty() const {
        return TextureObject<Const>::GetProperty(kRightPropertyName);
      }
    };
    typedef GenericCubeMapObject<true> ConstCubeMapObject;
    typedef GenericCubeMapObject<false> CubeMapObject;
  }

  namespace texture {
    class CubeMap : public TextureTemplate<kCubeMap> {
    protected:
      explicit CubeMap(const TextureId id);
    public:
      ~CubeMap() override;
      std::string ToString() const override;
    private:
      static inline CubeMap*
      New(const TextureId id) {
        return new CubeMap(id);
      }
    public:
      static inline CubeMap*
      New(const CubeMapBuilder& builder) {
        return New(builder.Build());
      }

      static CubeMap* New(const uri::Uri& uri);

      static inline CubeMap*
      New(const uri::basic_uri& uri) {
        return New(uri::Uri(uri));
      }
    };

    class CubeMapLoader : public TextureLoader<CubeMap> {
    protected:
      CubeMapLoader() = default;
    public:
      ~CubeMapLoader() override = default;
    };
  }
  using texture::CubeMap;
}

#endif //MCC_CUBE_MAP_H