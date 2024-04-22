#ifndef MCC_RENDER_TIMER_H
#define MCC_RENDER_TIMER_H

#include <units.h>
#include <functional>

#include "mcc/common.h"
#include "mcc/uv/utils.h"

namespace mcc::render {
  typedef int32_t FramesPerSecond;

  class Renderer;
  class RenderTimer {
    friend class OnTickCallback;
  public:
    struct Tick {
      uint64_t ts;
      uint64_t last;

      friend std::ostream& operator<<(std::ostream& stream, const Tick& rhs) {
        stream << "RenderTimer::Tick(";
        stream << "ts=" << rhs.ts << ", ";
        stream << "last=" << rhs.last;
        stream << ")";
        return stream;
      }
    };

    typedef std::function<void(const Tick&)> OnTick;
  private:
    class OnTickCallback {
    private:
      const RenderTimer* timer_;
      uv_async_t handle_;
      OnTick callback_;

      inline const RenderTimer* GetTimer() const {
        return timer_;
      }

      inline uv_loop_t* GetLoop() const {
        return GetTimer()->GetLoop();
      }

      inline const Tick& GetTick() const {
        return GetTimer()->GetTick();
      }

      inline void InitAsyncHandle() {
        const auto result = uv_async_init(GetLoop(), &handle_, &OnSend);
        LOG_IF(ERROR, result != UV_OK) << "failed to initialize RenderTimer::OnTickCallback async handle: " << uv_strerror(result);
      }

      template<typename H>
      static inline void
      SetCallback(H* handle, const OnTickCallback* data) {
        uv_handle_set_data((uv_handle_t*) handle, (void*) data);
      }

      template<typename H>
      static inline void
      SetCallback(H& handle, const OnTickCallback* data) {
        return SetCallback<H>(&handle, data);
      }

      template<typename H>
      static inline OnTickCallback*
      GetCallback(const H* handle) {
        return (OnTickCallback*) uv_handle_get_data((uv_handle_t*) handle);
      }

      static inline void
      OnSend(uv_async_t* handle) {
        const auto callback = GetCallback(handle);
        (*callback)(callback->GetTick());
      }
    public:
      OnTickCallback(const RenderTimer* timer, const OnTick& callback):
        timer_(timer),
        handle_(),
        callback_(callback) {
        MCC_ASSERT(timer);
        SetCallback(handle_, this);
        InitAsyncHandle();
      }
      ~OnTickCallback() = default;

      void Send() {
        const auto result = uv_async_send(&handle_);
        LOG_IF(ERROR, result != UV_OK);
      }

      void operator()(const RenderTimer::Tick& tick) {
        return callback_(tick);
      }
    };
  protected:
    const Renderer* renderer_;
    uv_idle_t idle_;
    FramesPerSecond target_;
    float rate_limit_;
    Tick tick_;
    OnTickCallback callback_;

    inline const Renderer*
    GetRenderer() const {
      return renderer_;
    }

    template<typename H>
    static inline void
    SetTimer(H* handle, const RenderTimer* data) {
      uv_handle_set_data((uv_handle_t*) handle, (void*) data);
    }

    template<typename H>
    static inline void
    SetTimer(H& handle, const RenderTimer* data) {
      return SetTimer<H>(&handle, data);
    }

    template<typename H>
    static inline RenderTimer*
    GetTimer(const H* handle) {
      return (RenderTimer*) uv_handle_get_data((const uv_handle_t*) handle);
    }

    static void OnIdle(uv_idle_t* handle);

    static inline float
    CalculateRateLimit(const FramesPerSecond target) {
      return NSEC_PER_SEC / (static_cast<float>(target) * NSEC_PER_SEC);
    }

    void Send() {
      callback_.Send();
    }

    const Tick& GetTick() const {
      return tick_;
    }
  public:
    explicit RenderTimer(const Renderer* renderer,
                         const FramesPerSecond target,
                         const OnTick& callback);
    virtual ~RenderTimer();
    uv_loop_t* GetLoop() const;

    float GetRateLimit() const {
      return rate_limit_;
    }

    FramesPerSecond GetTarget() const {
      return target_;
    }
  };
}

#endif //MCC_RENDER_TIMER_H