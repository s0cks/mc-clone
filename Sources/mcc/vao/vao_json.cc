#include "mcc/vao/vao_json.h"
#include <glog/logging.h>

namespace mcc::vao {
  bool VaoReaderHandler::OnParseType(const std::string& type) {
    DLOG(INFO) << "parsed Vao type: " << type;
    return TransitionTo(kOpen);
  }

  bool VaoReaderHandler::OnParseMetaName(const std::string& name) {
    DLOG(INFO) << "parsed Vao name: " << name;
    return TransitionTo(kMeta);
  }

  bool VaoReaderHandler::OnParseMetaTag(const std::string& value) {
    DLOG(INFO) << "parsed Vao tag: " << value;
    return TransitionTo(kMetaTags);
  }
}