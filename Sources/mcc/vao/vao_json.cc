#include "mcc/vao/vao_json.h"
#include <glog/logging.h>

namespace mcc::vao {
  bool VaoReaderHandler::TransitionTo(const State state) {
    SetState(state);
    return state != kError;
  }

  bool VaoReaderHandler::Default() {
    return TransitionTo(kError);
  }

  bool VaoReaderHandler::StartObject() {
    switch(GetState()) {
      case kClosed:
        return TransitionTo(kOpen);
      default:
        return TransitionTo(kError);
    }
  }

  bool VaoReaderHandler::EndObject(const rapidjson::SizeType size) {
    switch(GetState()) {
      case kOpen:
        return TransitionTo(kClosed);
      default:
        return TransitionTo(kError);
    }
  }

  bool VaoReaderHandler::String(const char* value, const rapidjson::SizeType length, const bool) {
    switch(GetState()) {
      case kOpen:
        return OnParseField(std::string(value, length));
      default:
        return TransitionTo(kError);
    }
  }
  
  bool VaoReaderHandler::OnParseField(const std::string& name) {
    DLOG(INFO) << "unexpected field: " << name;
    return TransitionTo(kError);
  }
}