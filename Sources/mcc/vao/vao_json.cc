#include "mcc/vao/vao_json.h"
#include <glog/logging.h>

namespace mcc::vao {
  bool VaoReaderHandler::OnParseDataField(const std::string& name) {
    DLOG(ERROR) << "unexpected field: " << name;
    return TransitionTo(kError);
  }
}