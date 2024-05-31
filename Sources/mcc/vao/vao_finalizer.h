#ifndef MCC_VAO_FINALIZER_H
#define MCC_VAO_FINALIZER_H

#include "mcc/wsq.h"
#include "mcc/counter.h"
#include "mcc/platform.h"

namespace mcc::vao {
  class Vao;
  class VaoFinalizer {
  protected:
    Counter<uint32_t> num_finalized_;
    WorkStealingQueue<uword> queue_;

    VaoFinalizer() = default;
    
    virtual void Submit(Vao* vao) {
      queue_.Push(reinterpret_cast<uword>(vao));
    }

    void Finalize(Vao* vao);
    void FinalizeQueue();
  public:
    ~VaoFinalizer() = default;
  };
}

#endif //MCC_VAO_FINALIZER_H