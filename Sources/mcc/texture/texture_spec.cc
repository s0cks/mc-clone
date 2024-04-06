#include "mcc/texture/texture_spec.h"

namespace mcc::texture {
  class DefaultTextureSpec : public TextureSpec {
    friend class TextureSpec;
    DEFINE_NON_COPYABLE_TYPE(DefaultTextureSpec);
  protected:
    uri::Uri file_;
    TextureTarget target_;
    TextureWrap wrap_;
    TextureFilter filter_;
    PixelStoreAlignment alignment_;

    DefaultTextureSpec(const uri::Uri& file,
                       const TextureTarget& target,
                       const TextureWrap& wrap,
                       const TextureFilter& filter,
                       const PixelStoreAlignment& alignment):
      TextureSpec(),
      file_(file),
      target_(target),
      wrap_(wrap),
      filter_(filter),
      alignment_(alignment) {
    }
  public:
    ~DefaultTextureSpec() override = default;
    
    std::optional<uri::Uri> GetFile() const override {
      return { file_ };
    }

    std::optional<TextureTarget> GetTarget() const override {
      return { target_ };
    }

    std::optional<TextureWrap> GetWrap() const override {
      return { wrap_ };
    }

    std::optional<TextureFilter> GetFilter() const override {
      return { filter_ };
    }

    std::optional<PixelStoreAlignment> GetAlignment() const override {
      return { alignment_ };
    }
  };

  rx::observable<TextureSpec*> TextureSpec::New(const uri::Uri& file,
                                                const TextureTarget& target,
                                                const TextureWrap& wrap,
                                                const TextureFilter& filter,
                                                const PixelStoreAlignment& alignment) {
    return rx::observable<>::just<TextureSpec*>(new DefaultTextureSpec(file, target, wrap, filter, alignment));
  }

  class JsonTextureSpec : public TextureSpec, protected json::JsonSpec {
    friend class TextureSpec;
    DEFINE_NON_COPYABLE_TYPE(JsonTextureSpec);
  protected:
    static constexpr const auto kFilePropertyName = "file";
    static constexpr const auto kTargetPropertyName = "target";
    static constexpr const auto kWrapPropertyName = "wrap";
    static constexpr const auto kFilterPropertyName = "filter";
    static constexpr const auto kAlignmentPropertyName = "alignment";

    JsonTextureSpec(const uri::Uri& uri, const json::DocumentPtr& doc):
      TextureSpec(),
      json::JsonSpec(uri, doc) {
    }
  public:
    ~JsonTextureSpec() override = default;
    std::optional<uri::Uri> GetFile() const override;
    
    std::optional<TextureTarget> GetTarget() const override {
      if(!doc()->HasMember(kTargetPropertyName))
        return std::nullopt;
      const auto& property = (*doc())[kTargetPropertyName];
      if(!property.IsString())
        return std::nullopt;
      const std::string val(property.GetString(), property.GetStringLength());
      return ParseTextureTarget(val);
    }

    std::optional<TextureWrap> GetWrap() const override;
    std::optional<TextureFilter> GetFilter() const override;
    std::optional<PixelStoreAlignment> GetAlignment() const override;

    friend std::ostream& operator<<(std::ostream& stream, const JsonTextureSpec& rhs) {
      stream << "JsonTextureSpec(";
      stream << ")";
      return stream;
    }
  public:
    static inline rx::observable<JsonTextureSpec*>
    Parse(const uri::Uri& uri) {
      return json::ParseDocument(uri)
        .map([&uri](const json::DocumentPtr& doc) {
          return new JsonTextureSpec(uri, doc);
        });
    }
  };

  rx::observable<TextureSpec*> TextureSpec::FromJson(const uri::Uri& uri) {
    return json::ParseDocument(uri)
        .map([&uri](const json::DocumentPtr& doc) {
          return (TextureSpec*) new JsonTextureSpec(uri, doc);
        });
  }

  std::optional<uri::Uri> JsonTextureSpec::GetFile() const {
    return GetUriProperty(kFilePropertyName);
  }

  std::optional<TextureWrap> JsonTextureSpec::GetWrap() const {
    if(!doc_->HasMember(kWrapPropertyName))
      return std::nullopt;
    const auto& property = (*doc_)[kWrapPropertyName];
    if(property.IsString()) {
      const std::string value(property.GetString(), property.GetStringLength());
      return { TextureWrap(GetWrapMode(value).value_or(kDefaultTextureWrapMode)) };
    } else if(property.IsObject()) {
      return { TextureWrap(property.GetObject()) };
    }
    DLOG(WARNING) << kWrapPropertyName << " is not of type: object,string";
    return std::nullopt;
  }

  std::optional<TextureFilter> JsonTextureSpec::GetFilter() const {
    if(!doc_->HasMember(kFilterPropertyName))
      return std::nullopt;
    const auto& property = (*doc_)[kFilterPropertyName];
    if(property.IsString()) {
      const std::string value(property.GetString(), property.GetStringLength());
      return { TextureFilter(value) };
    } else if(property.IsObject()) {
      return { TextureFilter(property.GetObject()) };
    }
    DLOG(WARNING) << kFilterPropertyName << " is not of type: object,string";
    return std::nullopt;
  }

  std::optional<PixelStoreAlignment> JsonTextureSpec::GetAlignment() const {
    NOT_IMPLEMENTED(ERROR);
    return std::nullopt;
  }
}