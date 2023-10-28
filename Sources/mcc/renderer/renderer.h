#ifndef MCC_RENDERER_H
#define MCC_RENDERER_H

#include "mcc/common.h"
#include "mcc/ecs/system.h"
#include "mcc/renderer/renderable.h"

namespace mcc {
#define FOR_EACH_RENDERER_STATE(V) \
  V(PreRender)                     \
  V(Render)                        \
  V(PostRender)

  class Renderer {
    DEFINE_NON_INSTANTIABLE_TYPE(Renderer);
  public:
    enum State {
#define DEFINE_RENDERER_STATE(Name) k##Name,
      FOR_EACH_RENDERER_STATE(DEFINE_RENDERER_STATE)
#undef DEFINE_RENDERER_STATE
    };
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
    static void OnTick(const Tick& tick);
    static void RenderEntity(const glm::mat4 projection, const glm::mat4 view, const Entity e);

    static void PreRender();
    static void PostRender();
    static void SetState(const State state);
  public:
    static State GetState();
    static void Init();
    static uint64_t GetFrameCount();
    static uint64_t GetFPS();

#define DEFINE_STATE_CHECK(Name) \
    static inline bool Is##Name() { return GetState() == State::k##Name; }
    FOR_EACH_RENDERER_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK
  };
}

#endif //MCC_RENDERER_H