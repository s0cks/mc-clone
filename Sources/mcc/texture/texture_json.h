#ifndef MCC_TEXTURE_JSON_H
#define MCC_TEXTURE_JSON_H

namespace mcc::json {
  template<const bool Const>
  class GenericTextureObject {
  public:
    static constexpr const auto kFilePropertyName = "file"; //TODO: move to tex1d,tex2d,tex3d
    static constexpr const auto kFilterPropertyName = "filter";
    static constexpr const auto kWrapPropertyName = "wrap";
  private:
    typedef GenericObject<Const, Document::ValueType> ObjectType;
  protected:
    const ObjectType& value_;

    inline const ObjectType& value() const {
      return value_;
    }
  protected:
    inline std::optional<const Value*>
    GetProperty(const char* name) const {
      if(!value_.HasMember(name))
        return std::nullopt;
      return { &value_[name] };
    }
  public:
    explicit GenericTextureObject(const ObjectType& value):
      value_(value) {
    }
    virtual ~GenericTextureObject() = default;

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
  typedef GenericTextureObject<true> ConstTextureObject;
  typedef GenericTextureObject<false> TextureObject;
}

#endif //MCC_TEXTURE_JSON_H