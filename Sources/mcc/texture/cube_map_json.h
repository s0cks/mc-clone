#ifndef MCC_CUBE_MAP_JSON_H
#define MCC_CUBE_MAP_JSON_H

#include "mcc/texture/texture_json.h"

namespace mcc::json {
  template<const bool Const>
  class GenericCubeMapObject : public GenericTextureObject<Const> {
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
      GenericTextureObject<Const>(value) {
    }
    ~GenericCubeMapObject() override = default;

    virtual std::optional<const Value*> GetFrontProperty() const {
      return GenericTextureObject<Const>::GetProperty(kFrontPropertyName);
    }

    virtual std::optional<const Value*> GetBackProperty() const {
      return GenericTextureObject<Const>::GetProperty(kBackPropertyName);
    }

    virtual std::optional<const Value*> GetTopProperty() const {
      return GenericTextureObject<Const>::GetProperty(kTopPropertyName);
    }

    virtual std::optional<const Value*> GetBottomProperty() const {
      return GenericTextureObject<Const>::GetProperty(kBottomPropertyName);
    }

    virtual std::optional<const Value*> GetLeftProperty() const {
      return GenericTextureObject<Const>::GetProperty(kLeftPropertyName);
    }

    virtual std::optional<const Value*> GetRightProperty() const {
      return GenericTextureObject<Const>::GetProperty(kRightPropertyName);
    }
  };
  typedef GenericCubeMapObject<true> ConstCubeMapObject;
  typedef GenericCubeMapObject<false> CubeMapObject;
}

#endif //MCC_CUBE_MAP_JSON_H