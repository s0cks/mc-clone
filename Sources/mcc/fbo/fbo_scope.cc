#include "mcc/fbo/fbo_scope.h"
#include "mcc/fbo/fbo.h"

namespace mcc::fbo {
  FboId FboScope::GetFboId() const {
    return GetFbo()->GetId();
  }
}