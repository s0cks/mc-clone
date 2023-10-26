#ifndef MCC_ENGINE_STATE_H
#define MCC_ENGINE_STATE_H

#include <iostream>
#include <functional>
#include <glog/logging.h>
#include "mcc/engine/tick.h"

namespace mcc::engine {
#define FOR_EACH_ENGINE_STATE(V) \
  V(PreInit)                     \
  V(Init)                        \
  V(PostInit)                    \
  V(PreTick)                     \
  V(Tick)                        \
  V(PostTick)                    \
  V(Terminating)                 \
  V(Terminated)

  enum State {
    kUninitialized,
#define DEFINE_STATE(Name) k##Name,
    FOR_EACH_ENGINE_STATE(DEFINE_STATE)
#undef DEFINE_STATE
  };

  static inline std::ostream&
  operator<<(std::ostream& stream, const State& rhs) {
    switch(rhs) {
      case kUninitialized: return stream << "Uninitialized";
#define DEFINE_TOSTRING(Name) case State::k##Name: return stream << #Name;
      FOR_EACH_ENGINE_STATE(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      default: return stream << "Unknown";
    }
  }

  typedef std::function<void()> EngineStateCallback;

  class EngineStateCallbackHandle {
  protected:
    uv_async_t handle_;

    static inline void
    OnCall(uv_async_t* handle) {
      ((EngineStateCallbackHandle*) handle->data)->OnState();
    }

    static inline void
    OnClosed(uv_handle_t* h) {
      DLOG(INFO) << "handle closed.";
    }

    virtual void OnState() = 0;
  public:
    explicit EngineStateCallbackHandle(uv_loop_t* loop):
      handle_() {
      uv_async_init(loop, &handle_, &OnCall);
      handle_.data = this;
    }
    virtual ~EngineStateCallbackHandle() = default;

    void Send() {
      uv_async_send(&handle_);
    }

    void Close() {
      uv_close((uv_handle_t*) &handle_, &OnClosed);
    }
  };

  template<typename Callback>
  class EngineStateCallbackHandleTemplate : public EngineStateCallbackHandle {
  protected:
    Callback callback_;

    EngineStateCallbackHandleTemplate(uv_loop_t* loop, Callback callback):
      EngineStateCallbackHandle(loop),
      callback_(callback) {
    }

    void OnState() override {
      callback_();
    }
  public:
    ~EngineStateCallbackHandleTemplate() override = default;
  };

  typedef std::function<void()> PreInitCallback;
  class PreInitCallbackHandle : public EngineStateCallbackHandleTemplate<PreInitCallback> {
  public:
    PreInitCallbackHandle(uv_loop_t* loop, PreInitCallback callback):
      EngineStateCallbackHandleTemplate(loop, callback) {
    }
    ~PreInitCallbackHandle() override = default;
  };

  typedef std::function<void()> InitCallback;
  class InitCallbackHandle : public EngineStateCallbackHandleTemplate<InitCallback> {
  public:
    InitCallbackHandle(uv_loop_t* loop, InitCallback callback):
      EngineStateCallbackHandleTemplate(loop, callback) {
    }
    ~InitCallbackHandle() override = default;
  };

  typedef std::function<void()> PostInitCallback;
  class PostInitCallbackHandle : public EngineStateCallbackHandleTemplate<PostInitCallback> {
  public:
    PostInitCallbackHandle(uv_loop_t* loop, PostInitCallback callback):
      EngineStateCallbackHandleTemplate(loop, callback) {
    }
    ~PostInitCallbackHandle() override = default;
  };

  typedef std::function<void()> PreTickCallback;
  class PreTickCallbackHandle : public EngineStateCallbackHandleTemplate<PreTickCallback> {
  public:
    PreTickCallbackHandle(uv_loop_t* loop, PreTickCallback callback):
      EngineStateCallbackHandleTemplate(loop, callback) {
    }
    ~PreTickCallbackHandle() override = default;
  };

  typedef std::function<void(const Tick&)> TickCalback;
  class TickCallbackHandle : public EngineStateCallbackHandle {
  protected:
    TickCallback callback_;

    void OnState() override;
  public:
    TickCallbackHandle(uv_loop_t* loop, TickCallback callback):
      EngineStateCallbackHandle(loop),
      callback_(callback) {
    }
    ~TickCallbackHandle() override = default;
  };

  typedef std::function<void()> PostTickCallback;
  class PostTickCallbackHandle : public EngineStateCallbackHandleTemplate<PostTickCallback> {
  public:
    PostTickCallbackHandle(uv_loop_t* loop, PostTickCallback callback):
      EngineStateCallbackHandleTemplate(loop, callback) {
    }
    ~PostTickCallbackHandle() override = default;
  };

  typedef std::function<void()> TerminatingCallback;
  class TerminatingCallbackHandle : public EngineStateCallbackHandleTemplate<TerminatingCallback> {
  public:
    TerminatingCallbackHandle(uv_loop_t* loop, TerminatingCallback callback):
      EngineStateCallbackHandleTemplate(loop, callback) {
    }
    ~TerminatingCallbackHandle() override = default;
  };

  typedef std::function<void()> TerminatedCallback;
  class TerminatedCallbackHandle : public EngineStateCallbackHandleTemplate<TerminatedCallback> {
  public:
    TerminatedCallbackHandle(uv_loop_t* loop, TerminatedCallback callback):
      EngineStateCallbackHandleTemplate(loop, callback) {
    }
    ~TerminatedCallbackHandle() override = default;
  };
}

#endif //MCC_ENGINE_STATE_H