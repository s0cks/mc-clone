#include "mcc/json.h"
#include "mcc/buffer.h"

namespace mcc::json {
// if (d.Parse(filename).HasParseError()) { 
// }
  bool ParseJson(FILE* file, Document& doc) {
    auto buffer = Buffer::New(4096);
    FileReadStream frs(file, (char*) buffer->data(), 4096);
    doc.ParseStream(frs);
    if(doc.HasParseError()) {
      fclose(file);
      return false;
    }
    
    fclose(file);
    return true;
  }

  bool ParseJson(Document& doc, const std::string& value) {
    StringStream ss(value.data());
    doc.ParseStream(ss);
    return !doc.HasParseError();
  }
}