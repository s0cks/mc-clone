#include "mcc/json.h"
#include "mcc/buffer.h"

namespace mcc {
  bool ParseJson(const std::string& filename, json::Document& doc) {
    DLOG(INFO) << "parsing json from " << filename;
    auto file = fopen(filename.c_str(), "rb");
    if(!file)
      return false;
    auto buffer = Buffer::New(4096);
    json::FileReadStream frs(file, (char*) buffer->data(), 4096);
    doc.ParseStream(frs);
    fclose(file);
    return true;
  }
}