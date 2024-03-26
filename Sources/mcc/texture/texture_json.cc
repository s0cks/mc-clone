#include <algorithm>
#include <functional>
#include <fmt/format.h>
#include <glog/logging.h>

#include "mcc/flags.h"
#include "mcc/thread_local.h"
#include "mcc/file_resolver.h"
#include "mcc/texture/texture_json.h"

namespace mcc::texture {
  class TextureSchemaDocument;
  typedef std::shared_ptr<TextureSchemaDocument> TextureSchema;

  class TextureSchemaDocument {
  protected:
    std::string filename_;
    json::SchemaDocument doc_;

    TextureSchemaDocument(const std::string& filename,
                          const json::Document& doc):
      filename_(filename),
      doc_(doc) {
    }
  public:
    virtual ~TextureSchemaDocument() = default;

    std::string filename() const {
      return filename_;
    }

    virtual bool IsValid(const json::Document& doc) const {
      json::SchemaValidator validator(doc_);
      return doc.Accept(validator);
    }

    virtual bool IsValid(const json::Value& value) const {
      MCC_ASSERT(value.IsObject());
      json::SchemaValidator validator(doc_);
      return value.Accept(validator);
    }

    friend std::ostream& operator<<(std::ostream& stream, const TextureSchemaDocument& rhs) {
      stream << "TextureSchemaDocument(";
      stream << "filename=" << rhs.filename_;
      stream << ")";
      return stream;
    }

    static inline TextureSchemaDocument*
    New(const std::string& filename) {
      json::Document doc;
      if(!json::ParseJson(filename, doc)) {
        DLOG(ERROR) << "failed to parse TextureSchemaDocument from: " << filename;
        return nullptr;
      }
      return new TextureSchemaDocument(filename, doc);
    }

    static inline TextureSchemaDocument*
    New() {
      const auto root = fmt::format("{0:s}/textures");
      PathResolver resolver(root, HasExtension(".schema.json"));
      const auto filename = resolver.Resolve();
      if(!filename) {
        DLOG(ERROR) << "cannot find TextureSchemaDocument in: " << root;
        return nullptr;
      }
      return New((*filename));
    }
  };

  static LazyThreadLocal<TextureSchemaDocument> kTextureSchemaDocument([]() {
    return TextureSchemaDocument::New();
  });

  static inline TextureSchemaDocument* GetTextureSchema() {
    return kTextureSchemaDocument.Get();
  }

  bool JsonTexture::IsValid() const {
    const auto schema = GetTextureSchema();
    if(!schema)
      return true;
    return schema->IsValid(value_);
  }
}