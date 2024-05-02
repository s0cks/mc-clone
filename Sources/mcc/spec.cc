#include "mcc/spec.h"

namespace mcc {
  SpecDocument::SpecDocument(const uri::Uri& uri):
    uri_(uri),
    doc_() {
    LOG_IF(ERROR, !json::ParseJson(uri, doc_)) << "failed to parse SpecDocument from: " << uri;
  }
}