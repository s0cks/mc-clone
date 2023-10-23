#ifndef MCC_ENGINE_H
#define MCC_ENGINE_H

#include "mcc/uv_utils.h"
#include "mcc/common.h"
#include "mcc/engine/tick.h"
#include "mcc/relaxed_atomic.h"

namespace mcc {
  namespace engine {
    typedef RelaxedAtomic<uint64_t> TickCounter;

#define FOR_EACH_ENGINE_STATE(V) \
  V(Uninitialized)               \
  V(Initialized)                 \
  V(PreTick)                     \
  V(Tick)                        \
  V(PostTick)

    class Engine {
      DEFINE_NON_INSTANTIABLE_TYPE(Engine);
    public:
      enum State {
#define DEFINE_STATE(Name) k##Name,
        FOR_EACH_ENGINE_STATE(DEFINE_STATE)
#undef DEFINE_STATE
      };
    private:
      static void PreTick();
      static void OnTick();
      static void PostTick();
      static void SetState(const State state);
    public:
      static void Init(uv_loop_t* loop = uv_loop_new());
      static uv_loop_t* GetLoop();
      static State GetState();

#define DEFINE_STATE_CHECK(Name) static inline bool Is##Name() { return GetState() == State::k##Name; }
      FOR_EACH_ENGINE_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK
      
      static void Register(TickCallback callback);

      static inline void
      Register(TickListener* listener) {
        return Register(std::bind(&TickListener::OnTick, listener, std::placeholders::_1));
      }

      static uint64_t GetTPS();
      static uint64_t GetTotalTicks();
    };
  }
  using engine::Engine;
}

#endif //MCC_ENGINE_H