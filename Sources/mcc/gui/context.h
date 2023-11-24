#ifndef MCC_GUI_CONTEXT_H
#define MCC_GUI_CONTEXT_H

#include "mcc/vao.h"
#include "mcc/vertex.h"

namespace mcc::gui {
  class Context {
  protected:
    VertexArrayObject vao_;
    d2::VertexBuffer vbo_;
  public:
    Context();
    virtual ~Context() = default;
  };
}

#endif //MCC_GUI_CONTEXT_H