#include "mcc/fbo/fbo_scope.h"
#include "mcc/fbo/fbo.h"

namespace mcc::fbo {
  void FboBindScope::Bind() const {
    Fbo::BindFbo(GetFboId());
  }

  void FboBindScope::Unbind() const {
    Fbo::BindDefaultFbo();
  }
}