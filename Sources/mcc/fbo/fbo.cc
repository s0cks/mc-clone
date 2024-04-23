#include "mcc/fbo/fbo.h"

namespace mcc::fbo {
  rx::subject<FboEvent*> events_;

  rx::observable<FboEvent*> OnFboEvent() {
    return events_.get_observable();
  }

  rx::observable<FboEvent*> OnFboEvent(const FboId id) {
    return OnFboEvent()
      .filter([id](FboEvent* event) {
        return event
            && event->GetFboId() == id;
      });
  }
}