#include "mcc/resource/resource.h"

namespace mcc::resource {
  static rx::subject<ResourceEvent*> events_;

  rx::observable<ResourceEvent*> OnResourceEvent() {
    return events_.get_observable();
  }

  void Resource::Publish(ResourceEvent* event) {
    return events_.get_subscriber().on_next(event);
  }
}