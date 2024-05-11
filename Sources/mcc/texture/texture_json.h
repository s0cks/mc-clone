#ifndef MCC_TEXTURE_JSON_H
#define MCC_TEXTURE_JSON_H

namespace mcc::json {
  class TextureValue {
    DEFINE_NON_COPYABLE_TYPE(TextureValue);
  public:
    static constexpr const auto kFilePropertyName = "file";
    static constexpr const auto kWrapPropertyName = "wrap";
    static constexpr const auto kFilterPropertyName = "filter";
  protected:
    const Value* value_;

    explicit TextureValue(const Value* value):
      value_(value) {
      MCC_ASSERT(value);
    }

    inline const Value* value() const {
      return value_;
    }

    inline std::optional<const Value*>
    GetProperty(const char* name) const {
      if(!value()->HasMember(name))
        return std::nullopt;
      return { &(*value())[name] };
    }
  public:
    virtual ~TextureValue() = default;

    bool IsString() const {
      return value()->IsString();
    }

    bool IsObject() const {
      return value()->IsObject();
    }

    std::optional<const Value*> GetFileProperty() const {
      return GetProperty(kFilePropertyName);
    }

    std::optional<const Value*> GetFilterProperty() const {
      return GetProperty(kFilterPropertyName);
    }

    std::optional<const Value*> GetWrapProperty() const {
      return GetProperty(kWrapPropertyName);
    }

    uri::Uri GetFile() const {
      if(IsString())
        return std::string(value()->GetString(), value()->GetStringLength());
      MCC_ASSERT(IsObject());
      const auto file = GetFileProperty();
      MCC_ASSERT(file);
      MCC_ASSERT((*file)->IsString());
      return std::string((*file)->GetString(), (*file)->GetStringLength());
    }
  };

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