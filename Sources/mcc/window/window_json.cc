#include "mcc/window/window_json.h"

namespace mcc::window {
  bool WindowReaderHandler::String(const char* value, const rapidjson::SizeType length, const bool b) {
    switch(GetState()) {
      case WindowReaderState::kSize:
        return OnParseSize(std::string(value, length));
      case WindowReaderState::kTitle:
        return OnParseTitle(std::string(value, length));
      default: return ReaderHandlerTemplate::String(value, length, b);
    }
  }

  bool WindowReaderHandler::OnParseDataField(const std::string& name) {
    if(EqualsIgnoreCase(name, "fullscreen")) {
      return TransitionToFullscreen();
    } else if(EqualsIgnoreCase(name, "resizable")) {
      return TransitionToResizable();
    } else if(EqualsIgnoreCase(name, "title")) {
      return TransitionToTitle();
    }

    DLOG(ERROR) << "unexpected field: " << name;
    return TransitionToError();
  }

  bool WindowReaderHandler::Bool(const bool value) {
    switch(GetState()) {
      case WindowReaderState::kFullscreen: return OnParseFullscreen(value);
      case WindowReaderState::kResizable: return OnParseResizable(value);
      default: return ReaderHandlerTemplate::Bool(value);
    }
  }

  bool WindowReaderHandler::OnParseSize(const std::string& value) {
    DLOG(INFO) << "parsed Window size: " << value;
    return true;
  }

  bool WindowReaderHandler::OnParseTitle(const std::string& value) {
    DLOG(INFO) << "parsed Window title: " << value;
    return true;
  }

  bool WindowReaderHandler::OnParseResizable(const bool value) {
    DLOG(INFO) << "parsed Window resizable: " << value;
    return true;
  }

  bool WindowReaderHandler::OnParseFullscreen(const bool value) {
    DLOG(INFO) << "parsed window fullscreen: " << value;
    return true;
  }
}