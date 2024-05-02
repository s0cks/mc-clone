#ifndef MCC_MATERIAL_DOCUMENT_H
#define MCC_MATERIAL_DOCUMENT_H

#include <optional>

#include "mcc/uri.h"
#include "mcc/json.h"
#include "mcc/common.h"

namespace mcc::material {
  class MaterialDocument {
    typedef const json::Value& Property;
  public:
    static constexpr const auto kAlbedoPropertyName = "albedo";
    static constexpr const auto kAoPropertyName = "ao";
    static constexpr const auto kHeightPropertyName = "height";
    static constexpr const auto kMetallicPropertyName = "metallic";
    static constexpr const auto kNormalPropertyName = "normal";
    static constexpr const auto kRoughnessPropertyName = "roughness";
  protected:
    uri::Uri uri_;
    json::Document doc_;
  public:
    explicit MaterialDocument(const uri::Uri& uri);
    virtual ~MaterialDocument() = default;

    uri::Uri GetUri() const {
      return uri_;
    }

    Property GetAlbedoProperty() const {
      return doc_[kAlbedoPropertyName];
    }

    Property GetAoProperty() const {
      return doc_[kAoPropertyName];
    }

    Property GetHeightProperty() const {
      return doc_[kHeightPropertyName];
    }

    Property GetMetallicProperty() const {
      return doc_[kMetallicPropertyName];
    }

    Property GetNormalProperty() const {
      return doc_[kNormalPropertyName];
    }

    Property GetRoughnessProperty() const {
      return doc_[kRoughnessPropertyName];
    }
  };
}

#endif //MCC_MATERIAL_DOCUMENT_H