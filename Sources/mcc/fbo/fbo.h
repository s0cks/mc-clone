#ifndef MCC_FRAMEBUFFER_H
#define MCC_FRAMEBUFFER_H

#include "mcc/fbo/fbo_id.h"
#include "mcc/fbo/fbo_events.h"

namespace mcc {
  namespace fbo {
    rx::observable<FboEvent*> OnFboEvent();

    class Fbo {
    protected:
      FboId id_;

      virtual void Destroy();
    private:
      explicit Fbo(const FboId id):
        id_(id) {
      }
    public:
      virtual ~Fbo() = default;
      virtual std::string ToString() const;

      FboId GetId() const {
        return id_;
      }
    };

    static inline rx::observable<FboEvent*>
    OnFboEvent(const FboId id) {
      
    }
  }
  using fbo::Fbo;
}

#endif //MCC_FRAMEBUFFER_H