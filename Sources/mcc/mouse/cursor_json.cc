#include "mcc/mouse/cursor_json.h"

namespace mcc::mouse {
  bool CursorReaderHandler::String(const char* value, const rapidjson::SizeType length, const bool b) {
    switch(GetState()) {
      case kImage: {
        const std::string raw_uri(value, length);

        uri::Uri uri;
        if(!uri::TryParseUri(uri, raw_uri, "img")) {
          LOG(ERROR) << "failed to parse Cursor image uri: " << raw_uri;
          return TransitionTo(kError);
        }
        return OnParseImage(uri);
      }
      default: return ReaderHandlerTemplate::String(value, length, b);
    }
  }

  bool CursorReaderHandler::OnParseDataField(const std::string& name) {
    if(EqualsIgnoreCase(name, "image"))
      return TransitionTo(kImage);
    DLOG(ERROR) << "unexpected field: " << name;
    return TransitionTo(kError);
  }

  bool CursorReaderHandler::OnParseImage(const uri::Uri& uri) {
    DLOG(INFO) << "parsed Cursor image: " << uri;
    return TransitionTo(kData);
  }
}