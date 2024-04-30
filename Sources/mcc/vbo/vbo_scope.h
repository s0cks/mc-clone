#ifndef MCC_VBO_SCOPE_H
#define MCC_VBO_SCOPE_H

#include "mcc/vbo/vbo_id.h"

namespace mcc::vbo {
  class Vbo;
  class VboScope {
  protected:
    Vbo* vbo_;
    
    explicit VboScope(Vbo* vbo):
      vbo_(vbo) {
      MCC_ASSERT(vbo);
    }
  public:
    virtual ~VboScope() = default;

    Vbo* GetVbo() const {
      return vbo_;
    }

    VboId GetVboId() const;
  };
}

#include "mcc/vbo/vbo_scope_bind.h"
#include "mcc/vbo/vbo_scope_read.h"
#include "mcc/vbo/vbo_scope_draw.h"
#include "mcc/vbo/vbo_scope_update.h"

#endif //MCC_VBO_SCOPE_H