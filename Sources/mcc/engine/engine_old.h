// #ifndef MCC_ENGINE_H
// #define MCC_ENGINE_H

// #include "mcc/rx.h"

// #include "mcc/uv_utils.h"
// #include "mcc/common.h"
// #include "mcc/engine/tick.h"
// #include "mcc/engine/engine_state.h"
// #include "mcc/relaxed_atomic.h"
// #include "mcc/engine/engine_stats.h"

// #include "mcc/engine/engine_event.h"

// namespace mcc {
//   namespace engine {
//     typedef RelaxedAtomic<uint64_t> TickCounter;

//     class Engine {
//       DEFINE_NON_INSTANTIABLE_TYPE(Engine);

//       friend class EnginePhase;
//       friend class Stage;
//     private:
//       static void SetRunning(const bool value = true);

//       static void OnLoopClosed(uv_handle_t* handle) {
//         //TODO: implement
//       }

//       static void SetState(State* state);
//     public:
//       static void Init(uv_loop_t* loop = uv_loop_new());
//       static void Run();
//       static void Shutdown();
//       static bool IsRunning();
//       static uv_loop_t* GetLoop();
//       static Tick GetTick();
//       static SampleSeries* GetSamples();
//       static rx::observable<EngineStateEvent*> on_state();

//       static State* GetState();

// #define DEFINE_STATE_CHECK(Name) static inline bool Is##Name() { return GetState()->GetId() == StateId::k##Name; }
//       FOR_EACH_ENGINE_STATE(DEFINE_STATE_CHECK)
// #undef DEFINE_STATE_CHECK

//       static uint64_t GetTPS();
//       static uint64_t GetTotalTicks();
//     };
//   }
//   using engine::Engine;
// }

// #endif //MCC_ENGINE_H