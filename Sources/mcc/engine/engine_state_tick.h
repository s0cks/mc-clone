#ifndef MCC_ENGINE_STATE_H
#error "Please #include <mcc/engine/engine_state.h> instead."
#endif //MCC_ENGINE_STATE_H

#ifndef MCC_ENGINE_STATE_TICK_H
#define MCC_ENGINE_STATE_TICK_H

#include "mcc/engine/engine_ticker.h"
#include "mcc/renderer/render_ticker.h"

#include "mcc/on_shutdown.h"

namespace mcc::render {
  class Renderer;
}

namespace mcc::engine {
  class TickState : public State,
                    public ShutdownListener {
    friend class Engine;
    friend class render::Renderer;

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
  private:
    void Stop();
  protected:
    EngineTicker ticker_;
    render::RenderTicker render_ticker_;
    ShutdownListenerHandle shutdown_;

    explicit TickState(Engine* engine);

    void Shutdown() override;
    void OnShutdown() override;
  public:
    ~TickState() override = default;

    const uv::TickDurationSeries& GetTickDurationSeries() const {
      return ticker_.GetTickDurationSeries();
    }

    const uv::TicksPerSecond& GetTicksPerSecond() const {
      return ticker_.GetTicksPerSecond();
    }

    DECLARE_STATE(Tick);
  };
}

#endif //MCC_ENGINE_STATE_TICK_H