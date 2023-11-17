#include "mcc/json.h"
#include "mcc/buffer.h"

namespace mcc::json {
// if (d.Parse(filename).HasParseError()) { 
// }
  bool ParseJson(FILE* file, Document& doc) {
    auto buffer = Buffer::New(4096);
    FileReadStream frs(file, (char*) buffer->data(), 4096);
    doc.ParseStream(frs);
    fclose(file);
    return true;
  }
}