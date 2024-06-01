#include "mcc/vao/vao_json.h"
#include <glog/logging.h>

namespace mcc::vao {
  bool VaoReaderHandler::OnParseType(const std::string& type) {
    DLOG(INFO) << "parsed Vao type: " << type;
    return TransitionTo(kOpen);
  }

  bool VaoReaderHandler::OnParseDataField(const std::string& name) {
    DLOG(ERROR) << "unexpected field: " << name;
    return TransitionTo(kError);
  }
}