#ifndef MCC_MATERIAL_JSON_H
#define MCC_MATERIAL_JSON_H

#include "mcc/json.h"
#include "mcc/file_resolver.h"

namespace mcc::material {
  class MaterialDocument {
    DEFINE_NON_COPYABLE_TYPE(MaterialDocument);
    static constexpr const auto kNamePropertyName = "name";
    static constexpr const auto kDescriptionPropertyName = "description";
    static constexpr const auto kAlbedoPropertyName = "albedo";
    static constexpr const auto kAoPropertyName = "ao";
    static constexpr const auto kHeightPropertyName = "height";
    static constexpr const auto kMetallicPropertyName = "metallic";
    static constexpr const auto kNormalPropertyName = "normal";
    static constexpr const auto kRoughnessPropertyName = "roughness";
  protected:
    std::string filename_;
    json::Document doc_;

    std::optional<std::string> GetStringProperty(const char* name) const {
      if(!doc_.HasMember(name)) {
        DLOG(WARNING) << "no " << name << " property in " << filename_;
        return std::nullopt;
      }

      const auto& property = doc_[name];
      if(!property.IsString()) {
        DLOG(WARNING) << "expected " << name << " in " << filename_ << " to be a string.";
        return std::nullopt;
      }

      return { std::string(property.GetString(), property.GetStringLength()) };
    }
  public:
    MaterialDocument() = delete;
    explicit MaterialDocument(const std::string& filename):
      filename_(filename),
      doc_() {
      if(!json::ParseJson(filename, doc_)) {
        DLOG(ERROR) << "failed to parse material document from: " << filename;
        return;
      }
    }
    virtual ~MaterialDocument() = default;

    std::string filename() const {
      return filename_;
    }

    std::optional<std::string> GetAlbedoProperty() const {
      return GetStringProperty(kAlbedoPropertyName);
    }

    std::optional<std::string> GetAoProperty() const {
      return GetStringProperty(kAoPropertyName);
    }

    std::optional<std::string> GetHeightProperty() const {
      return GetStringProperty(kHeightPropertyName);
    }

    std::optional<std::string> GetMetallicProperty() const {
      return GetStringProperty(kMetallicPropertyName);
    }

    std::optional<std::string> GetNormalProperty() const {
      return GetStringProperty(kNormalPropertyName);
    }

    std::optional<std::string> GetRoughnessProperty() const {
      return GetStringProperty(kRoughnessPropertyName);
    }

    bool IsValid() const;

    friend std::ostream& operator<<(std::ostream& stream, const MaterialDocument& rhs) {
      stream << "MaterialDocument(";
      stream << "src=" << rhs.filename_ << ", ";
      stream << "valid=" << rhs.IsValid();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_MATERIAL_JSON_H