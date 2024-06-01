#ifndef MCC_VAO_BUILDER_H
#define MCC_VAO_BUILDER_H

#include "mcc/rx.h"
#include "mcc/builder.h"

namespace mcc::vao {
  class Vao;
  class VaoBuilder : public BuilderBase {
  public:
    VaoBuilder() = default;
    ~VaoBuilder() override = default;
    
    Vao* Build() const;
    rx::observable<Vao*> BuildAsync() const;
  };
}

#endif //MCC_VAO_BUILDER_H