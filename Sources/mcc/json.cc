#include "mcc/json.h"
#include "mcc/buffer.h"

namespace mcc {
  bool ParseJson(FILE* file, json::Document& doc) {
    auto buffer = Buffer::New(4096);
    json::FileReadStream frs(file, (char*) buffer->data(), 4096);
    doc.ParseStream(frs);
    fclose(file);
    return true;
  }
}