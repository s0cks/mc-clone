#ifndef MCC_RENDER_EVENTS_H
#define MCC_RENDER_EVENTS_H

#include "mcc/event.h"

namespace mcc::render {
#define FOR_EACH_RENDER_EVENT(V) \
  V(PreRender)                   \
  V(PostRender)                  \
  V(RenderModeChanged)

  class RenderEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_RENDER_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class RenderEvent : public Event {
  public:
    RenderEvent() = default;
    ~RenderEvent() override = default;
  };

  class PreRenderEvent : public Event {

  };

  class PostRenderEvent : public Event {

  };
}

#endif //MCC_RENDER_EVENTS_H