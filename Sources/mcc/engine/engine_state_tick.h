#ifndef MCC_ENGINE_STATE_H
#error "Please #include <mcc/engine/engine_state.h> instead."
#endif //MCC_ENGINE_STATE_H

#ifndef MCC_ENGINE_STATE_TICK_H
#define MCC_ENGINE_STATE_TICK_H

#include "mcc/renderer/render_timer.h"

namespace mcc::engine {
  class TickState : public State {
    friend class Engine;

    template<typename T>
    static inline void
    SetState(T* handle, const TickState* state) {
      uv_handle_set_data((uv_handle_t*) handle, (void*) state);
    }

    template<typename T>
    static inline TickState*
    GetState(T* handle) {
      return (TickState*) uv_handle_get_data((uv_handle_t*) handle);
    }
  protected:
    uv_idle_t idle_;
    uv_prepare_t prepare_;
    uv_check_t check_;
    uv::AsyncHandle<TickState> on_shutdown_;
    //render::RenderTimer render_timer_;

    explicit TickState(Engine* engine);

    void Shutdown() override;
    static void OnIdle(uv_idle_t* handle);
    static void OnPrepare(uv_prepare_t* handle);
    static void OnCheck(uv_check_t* handle);
    static void OnShutdown(TickState*);
    static void OnRenderTick(const render::RenderTimer::Tick& tick);
  public:
    ~TickState() override = default;
    DECLARE_STATE(Tick);
  };
}

#endif //MCC_ENGINE_STATE_TICK_H