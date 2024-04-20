#ifndef MCC_PROGRAM_DOCUMENT_H
#define MCC_PROGRAM_DOCUMENT_H

#include "mcc/uri.h"
#include "mcc/json.h"
#include "mcc/shader/shader_id.h"

namespace mcc::program {
  class ProgramDocument {
    static constexpr const auto kNameProperty = "name";
    static constexpr const auto kShadersProperty = "shaders";
  protected:
    uri::Uri uri_;
    json::Document doc_;

    inline json::Document& doc() {
      return doc_;
    }

    inline const json::Document& doc() const {
      return doc_;
    }
  public:
    explicit ProgramDocument(const uri::Uri& uri):
      uri_(uri),
      doc_() {
      MCC_ASSERT(uri.HasScheme("file"));
      MCC_ASSERT(uri.HasExtension(".json"));
      LOG_IF(ERROR, !json::ParseJson(uri, doc_)) << "failed to parse ProgramDocument from: " << uri;
    }
    virtual ~ProgramDocument() = default;

    uri::Uri uri() const {
      return uri_;
    }

    inline const json::Value& GetNameProperty() const {
      MCC_ASSERT(doc().HasMember(kNameProperty));
      return doc()[kNameProperty];
    }

    inline const json::Value& GetShadersProperty() const {
      MCC_ASSERT(doc().HasMember(kShadersProperty));
      return doc()[kShadersProperty];
    }

    friend std::ostream& operator<<(std::ostream& stream, const ProgramDocument& rhs) {
      stream << "ProgramDocument(";
      stream << "uri=" << rhs.uri();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_PROGRAM_DOCUMENT_H