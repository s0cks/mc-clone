#include "mcc/material/material_document.h"

namespace mcc::material {
  MaterialDocument::MaterialDocument(const uri::Uri& uri):
    uri_(uri),
    doc_() {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension(".json"));
    LOG_IF(ERROR, !json::ParseJson(uri, doc_)) << "failed to parse MaterialDocument from: " << uri;
  }
}