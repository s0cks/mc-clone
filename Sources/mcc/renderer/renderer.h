#ifndef MCC_RENDERER_H
#define MCC_RENDERER_H

#include "mcc/uv/utils.h"
#include "mcc/uv/uv_handle.h"

#include "mcc/rx.h"
#include "mcc/gfx.h"
#include "mcc/uv/uv_tick.h"
#include "mcc/relaxed_atomic.h"
#include "mcc/renderer/render_pass.h"
#include "mcc/renderer/render_events.h"
#include "mcc/renderer/renderer_state.h"
#include "mcc/renderer/renderer_stats.h"

namespace mcc {
  namespace engine {
    class TickState;
  }

  namespace render {
    class Renderer {
      friend class RenderTicker;
    public:
      static constexpr const auto kDefaultTargetFramesPerSecond = 60;

      enum Mode : GLenum {
        kFillMode = GL_FILL,
        kLineMode = GL_LINE,

        kNumberOfModes,
        // aliases
        kDefaultMode = kFillMode,
        kWireframeMode = kLineMode,
      };

      static void Publish(RenderEvent* event);

      template<class E, typename... Args>
      static inline void
      Publish(Args... args) {
        E event(args...);
        return Publish((RenderEvent*) &event);
      }
    private:
      template<typename H>
      static inline void
      SetRenderer(H* handle, const Renderer* value) {
        uv_handle_set_data((uv_handle_t*) handle, (void*) value);
      }

      template<typename H>
      static inline void
      SetRenderer(H& handle, const Renderer* value) {
        return SetRenderer<H>(&handle, value);
      }

      template<typename H>
      static inline Renderer*
      GetRenderer(const H* handle) {
        return (Renderer*) uv_handle_get_data((uv_handle_t*) handle);
      }

      static inline void
      InitAsyncHandle(uv_loop_t* loop,
                      uv_async_t& handle,
                      uv_async_cb cb) {
        MCC_ASSERT(loop);
        const auto err = uv_async_init(loop, &handle, cb);
        LOG_IF(ERROR, err != UV_OK) << "uv_async_init failed: " << uv_strerror(err);
      }
    private:
      uv_loop_t* loop_;
      uv_async_t on_run_;
      RelaxedAtomic<Mode> mode_;
      RenderPass* pass_;

      Renderer(uv_loop_t* loop,
               const Mode mode = kDefaultMode):
        loop_(loop),
        on_run_(),
        mode_(mode),
        pass_(new OrderedSequenceRenderPass()) {
        MCC_ASSERT(loop);
        SetRenderer(on_run_, this);
        InitAsyncHandle(loop, on_run_, &OnRun);
        Publish<RendererInitializedEvent>(this);
      }

      inline void SetMode(const Mode mode) {
        mode_ = mode;
      }

      inline void Schedule() {
        const auto err = uv_async_send(&on_run_);
        LOG_IF(ERROR, err != UV_OK) << "uv_async_send failed: " << uv_strerror(err);
      }

      virtual void Run(const uv_run_mode mode = UV_RUN_NOWAIT);

      static inline void
      OnRun(uv_async_t* handle) {
        const auto renderer = GetRenderer(handle);
        MCC_ASSERT(renderer);
        renderer->Run();
      }
    public:
      virtual ~Renderer() = default;

      Mode GetMode() const {
        return (Mode) mode_;
      }

      RenderPass* GetPass() const {
        return pass_;
      }

      uv_loop_t* GetLoop() const {
        return loop_;
      }

      const uv::TickDurationSeries& GetTickDurationSeries() const;
      const uv::TicksPerSecond& GetTicksPerSecond() const;
    public:
      static void Init();
      static Renderer* Get();
    };
  }
}

#endif //MCC_RENDERER_H