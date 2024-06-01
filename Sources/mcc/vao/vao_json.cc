#include "mcc/vao/vao_json.h"
#include <glog/logging.h>

namespace mcc::vao {
  bool VaoReaderHandler::Default() {
    return TransitionTo(kError);
  }

  bool VaoReaderHandler::StartArray() {
    switch(GetState()) {
      case kSpecMetaTags:
        return TransitionTo(kSpecMetaTags);
      default:
        return TransitionTo(kError);
    }
  }

  bool VaoReaderHandler::EndArray(const rapidjson::SizeType size) {
    switch(GetState()) {
      case kSpecMetaTags:
        return TransitionTo(kSpecMeta);
      default:
        return TransitionTo(kError);
    }
  }

  bool VaoReaderHandler::StartObject() {
    switch(GetState()) {
      case kClosed:
        return TransitionTo(kOpen);
      case kSpecMeta:
        return TransitionTo(kSpecMeta);
      default:
        return TransitionTo(kError);
    }
  }

  bool VaoReaderHandler::EndObject(const rapidjson::SizeType size) {
    switch(GetState()) {
      case kOpen:
        return TransitionTo(kClosed);
      case kSpecMeta:
        return TransitionTo(kOpen);
      default:
        return TransitionTo(kError);
    }
  }

  bool VaoReaderHandler::String(const char* value, const rapidjson::SizeType length, const bool) {
    switch(GetState()) {
      case kOpen:
      case kSpecMeta:
        return OnParseField(std::string(value, length));
      case kSpecType: {
        const auto type = std::string(value, length);
        DLOG(INFO) << "parsed spec type: " << type;
        MCC_ASSERT(EqualsIgnoreCase(type, "Vao"));
        return TransitionTo(kOpen);
      }
      case kSpecMetaName:
        return OnParseMetaName(std::string(value, length));
      case kSpecMetaTags:
        return OnParseMetaTag(std::string(value, length));
      default:
        return TransitionTo(kError);
    }
  }

  bool VaoReaderHandler::OnParseMetaName(const std::string& name) {
    DLOG(INFO) << "parsed Vao name: " << name;
    return TransitionTo(kSpecMeta);
  }

  bool VaoReaderHandler::OnParseMetaTag(const std::string& value) {
    DLOG(INFO) << "parsed Vao tag: " << value;
    return TransitionTo(kSpecMetaTags);
  }
  
  bool VaoReaderHandler::OnParseField(const std::string& name) {
    switch(GetState()) {
      case kOpen: {
        if(EqualsIgnoreCase(name, "type")) {
          return TransitionTo(kSpecType);
        } else if(EqualsIgnoreCase(name, "meta")) {
          return TransitionTo(kSpecMeta);
        }
        break;
      }
      case kSpecMeta: {
        if(EqualsIgnoreCase(name, "name")) {
          return TransitionTo(kSpecMetaName);
        } else if(EqualsIgnoreCase(name, "tags")) {
          return TransitionTo(kSpecMetaTags);
        }
        break;
      }
      default: break;
    }

    DLOG(INFO) << "unexpected field: " << name;
    return TransitionTo(kError);
  }
}