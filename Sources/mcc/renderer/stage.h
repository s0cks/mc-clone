#ifndef MCC_RENDERER_STAGE_H
#define MCC_RENDERER_STAGE_H

#include "mcc/gfx.h"
#include "mcc/relaxed_atomic.h"
#include "mcc/engine/tick.h"
#include "mcc/uv_utils.h"
#include "mcc/renderer/renderer_state.h"
#include "mcc/shader/shader.h"
#include "mcc/ecs/entity.h"

namespace mcc::renderer {
#define FORWARD_DECLARE_STAGE(Name) class Name##Stage;
  FOR_EACH_RENDERER_STATE(FORWARD_DECLARE_STAGE);
#undef FORWARD_DECLARE_STAGE

  class Stage {
  protected:
    uv_loop_t* loop_;

    explicit Stage(uv_loop_t* loop):
      loop_(loop) {
    }
  public:
    virtual ~Stage() = default;
    virtual const char* GetName() const = 0;
    virtual RendererState GetState() const = 0;

    uv_loop_t* loop() const {
      return loop_;
    }
  };

#define DEFINE_STAGE(Name)                                              \
  public:                                                               \
    const char* GetName() const override { return #Name "Stage"; }      \
    RendererState GetState() const override { return k##Name##State; }

  class PreInitStage : public Stage {
    friend class Renderer;
  private:
    explicit PreInitStage(uv_loop_t* loop):
      Stage(loop) {
    }
  public:
    ~PreInitStage() override = default;
    DEFINE_STAGE(PreInit);
  };

  class InitStage : public Stage {
    friend class Renderer;
  private:
    explicit InitStage(uv_loop_t* loop):
      Stage(loop) {
    }
  public:
    ~InitStage() override = default;
    DEFINE_STAGE(Init);
  };

  class PostInitStage : public Stage {
    friend class Renderer;
  private:
    explicit PostInitStage(uv_loop_t* loop):
      Stage(loop) {
    }
  public:
    ~PostInitStage() override = default;
    DEFINE_STAGE(PostInit);
  };

  class LoopStage : public Stage {
  protected:
    explicit LoopStage(uv_loop_t* loop):
      Stage(loop) {
    }
  public:
    ~LoopStage() override = default;
  };

  template<typename Handle>
  class LoopStageTemplate : public LoopStage {
  protected:
    Handle handle_;

    explicit LoopStageTemplate(uv_loop_t* loop):
      LoopStage(loop),
      handle_() {
    }

    template<class S>
    static inline void
    SetHandleStage(Handle* handle, const S* stage) {
      uv_handle_set_data((uv_handle_t*) handle, (void*) stage);
    }

    template<class S>
    static inline S*
    GetHandleStage(Handle* handle) {
      return (S*) uv_handle_get_data((uv_handle_t*) handle);
    }
  public:
    ~LoopStageTemplate() override = default;
  };

  class PreRenderStage : public LoopStageTemplate<uv_idle_t> {
    friend class Renderer;
  private:
    explicit PreRenderStage(uv_loop_t* loop):
      LoopStageTemplate(loop) {
      CHECK_UV_RESULT(FATAL, uv_idle_init(loop, &handle_), "uv_prepare_init failed");
      CHECK_UV_RESULT(FATAL, uv_idle_start(&handle_, &OnPrepare), "uv_prepare_start failed");
      SetHandleStage(&handle_, this);
    }

    static void OnPrepare(uv_idle_t* handle);
  public:
    ~PreRenderStage() override = default;
    DEFINE_STAGE(PreRender);
  };

  class PostRenderStage : public LoopStageTemplate<uv_check_t> {
    friend class Renderer;
  private:
    explicit PostRenderStage(uv_loop_t* loop):
      LoopStageTemplate(loop) {
      CHECK_UV_RESULT(FATAL, uv_check_init(loop, &handle_), "uv_check_init failed");
      CHECK_UV_RESULT(FATAL, uv_check_start(&handle_, &OnCheck), "uv_check_start failed");
      SetHandleStage(&handle_, this);
    }

    static void OnCheck(uv_check_t* handle);
  public:
    ~PostRenderStage() override = default;
    DEFINE_STAGE(PostRender);
  };

  class RenderStage : public LoopStageTemplate<uv_prepare_t> {
  private:
    static void OnPrepare(uv_prepare_t* handle);
  protected:
    explicit RenderStage(uv_loop_t* loop):
      LoopStageTemplate(loop) {
      CHECK_UV_RESULT(FATAL, uv_prepare_init(loop, &handle_), "uv_check_init failed");
      CHECK_UV_RESULT(FATAL, uv_prepare_start(&handle_, &OnPrepare), "uv_check_start failed");
      SetHandleStage(&handle_, this);
    }

    virtual void Render(const Tick& tick) = 0;
  public:
    ~RenderStage() override = default;
  };

#define DEFINE_RENDER_STAGE(Name)                                                               \
  protected:                                                                                    \
    void Render(const Tick& tick) override; \
  DEFINE_STAGE(Render##Name);                                                                           

  class RenderTerrainStage : public RenderStage {
  public:
    explicit RenderTerrainStage(uv_loop_t* loop):
      RenderStage(loop) {
    }
    ~RenderTerrainStage() override = default;
    DEFINE_RENDER_STAGE(Terrain);
  };

  class RenderEntitiesStage : public RenderStage {
  protected:
    void RenderEntity(const Entity e);
  public:
    explicit RenderEntitiesStage(uv_loop_t* loop):
      RenderStage(loop) {
    }
    ~RenderEntitiesStage() override = default;
    DEFINE_RENDER_STAGE(Entities);
  };

  class RenderGuiStage : public RenderStage {
  public:
    explicit RenderGuiStage(uv_loop_t* loop):
      RenderStage(loop) {
    }
    ~RenderGuiStage() override = default;
    DEFINE_RENDER_STAGE(Gui);
  };

  class RenderScreenStage : public RenderStage {
  private:
    ShaderRef shader_;
  public:
    explicit RenderScreenStage(uv_loop_t* loop):
      RenderStage(loop),
      shader_(GetShader("framebuffer")) {
    }
    ~RenderScreenStage() override = default;
    DEFINE_RENDER_STAGE(Screen);
  };
}

#undef DEFINE_STAGE

#endif //MCC_RENDERER_STAGE_H