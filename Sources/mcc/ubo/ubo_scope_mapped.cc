#include "mcc/ubo/ubo_scope.h"
#include "mcc/ubo/ubo.h"

namespace mcc::ubo {
  UboMappedScope::UboMappedScope(Ubo* ubo, const Access access):
    UboBindScope(ubo),
    start_(0),
    access_(access) {
    switch(access) {
      case kReadOnly:
      case kWriteOnly: {
        const auto map = glMapBuffer(Ubo::kGlTarget, access);
        if(!map) {
          LOG(ERROR) << "failed to map gl buffer:";
          CHECK_GL(FATAL);
          return;
        }

        start_ = (uword) map;
        break;
      }
      case kNone:
      default: break;
    }
  }

  UboMappedScope::~UboMappedScope() {
    if(IsMapped()) {
      glUnmapBuffer(Ubo::kGlTarget);
      CHECK_GL(FATAL);
      start_ = 0;
    }
  }

  uint64_t UboMappedScope::GetSize() const {
    return GetUbo()->GetTotalSize();
  }
}