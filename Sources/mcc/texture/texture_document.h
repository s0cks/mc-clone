#ifndef MCC_TEXTURE_DOCUMENT_H
#define MCC_TEXTURE_DOCUMENT_H

#include "mcc/texture/texture_wrap.h"
#include "mcc/texture/texture_filter.h"
#include "mcc/texture/texture_target.h"

namespace mcc::texture {
  class TextureDocument {
  public:
    static constexpr const auto kWrapPropertyName = "wrap";
    static constexpr const auto kFilterPropertyName = "filter";
  protected:
    const json::ConstObject& doc_;

    explicit TextureDocument(const json::ConstObject& doc):
      doc_(doc) {
    }

    inline const json::ConstObject& doc() const {
      return doc_;
    }
  public:
    virtual ~TextureDocument() = default;
    virtual TextureTarget GetTarget() const = 0;

    const json::Value& GetTextureWrapProperty() const {
      MCC_ASSERT(doc().HasMember(kWrapPropertyName));
      return doc()[kWrapPropertyName];
    }

    const json::Value& GetTextureFilterProperty() const {
      MCC_ASSERT(doc().HasMember(kFilterPropertyName));
      return doc()[kFilterPropertyName];
    }

    bool Is2D() const {
      return GetTarget() == k2D;
    }

    bool Is3D() const {
      return GetTarget() == k3D;
    }

    bool IsCubeMap() const {
      return GetTarget() == kCubeMap;
    }
  public:
    static TextureDocument* New(const uri::Uri& uri);

    static inline TextureDocument*
    New(const uri::basic_uri& uri) {
      return New(uri::Uri(uri));
    }
  };

  template<const TextureTarget Target>
  class TextureDocumentTemplate : public TextureDocument {
  protected:
    explicit TextureDocumentTemplate(const json::ConstObject& doc):
      TextureDocument(doc) {
    }
  public:
    ~TextureDocumentTemplate() override = default;

    TextureTarget GetTarget() const override {
      return Target;
    }
  };

  class Texture2DDocument : public TextureDocumentTemplate<k2D> {
  protected:
    explicit Texture2DDocument(const json::ConstObject& doc):
      TextureDocumentTemplate<k2D>(doc) {
    }
  public:
    ~Texture2DDocument() override = default;
  public:
    static inline TextureDocument*
    New(const json::ConstObject& doc) {
      return new Texture2DDocument(doc);
    }

    static inline TextureDocument*
    New(const json::Value& doc) {
      return doc.IsObject()
           ? New(doc.GetObject())
           : nullptr;
    }
  };
}

#endif //MCC_TEXTURE_DOCUMENT_H