#ifndef MCC_RENDERER_H
#define MCC_RENDERER_H

#include "mcc/common.h"
#include "mcc/mesh/mesh.h"
#include "mcc/state_machine.h"

#include "mcc/renderer/render_timer.h"
#include "mcc/renderer/renderer_state.h"
#include "mcc/renderer/renderer_stats.h"


#include "mcc/engine/tick.h"
#include "mcc/entity/entity.h"
#include "mcc/framebuffer/framebuffer.h"

#include "mcc/engine/engine.h"

#include "mcc/skybox.h"
#include "mcc/camera/perspective_camera.h"

namespace mcc {
  namespace gui {
    class Screen;
    class Frame;
    class SettingsFrame;
    class RendererFrame;
  }

  namespace skybox {
    class RenderSkyboxPipeline;
  }

  namespace terrain {
    class RenderTerrainChunkPipeline;
  }
}

namespace mcc::renderer {
  class Renderer {
    friend class gui::Screen;
    friend class gui::SettingsFrame;
    friend class gui::RendererFrame;

    friend class engine::TickState;
    friend class PreRenderStage;
    friend class RenderStage;
    friend class PostRenderStage;
    friend class RenderTerrainStage;
    friend class RenderEntitiesStage;
    friend class RenderEntityPipeline;
    friend class RendererPipeline;
    friend class mcc::skybox::RenderSkyboxPipeline;
    friend class mcc::terrain::RenderTerrainChunkPipeline;
    DECLARE_STATE_MACHINE(Renderer);
  public:
    enum Mode {
      kFillMode = GL_FILL,
      kWireframeMode = GL_LINE,

      kNumberOfModes,
      kDefaultMode = kFillMode,
    };

    static constexpr const int16_t kDefaultTargetFps = 60;
  private:
    static void OnPreInit(engine::PreInitEvent* e);
    static void OnPostInit(engine::PostInitEvent* e);
    static void SetLoop(uv_loop_t* loop);
    static void SetMode(const Mode mode);

    static inline void
    ResetMode() {
      return SetMode(kDefaultMode);
    }

    static void IncrementEntityCounter(const uint64_t value = 1);
    static void DecrementEntityCounter(const uint64_t value = 1);
    static void SetEntityCounter(const uint64_t value);

    static inline void
    ResetEntityCounter() {
      return SetEntityCounter(0);
    }

    static void IncrementVertexCounter(const uint64_t value = 1);
    static void DecrementVertexCounter(const uint64_t value = 1);
    static void SetVertexCounter(const uint64_t value);

    static inline void
    ResetVertexCounter() {
      return SetVertexCounter(0);
    }

    static void Run(const uv_run_mode = UV_RUN_NOWAIT);
    static void OnRender(uv_async_t* handle);
    static void Schedule();
  public:
    static void Init();
    static uint64_t GetFrameCount();
    static uint64_t GetFPS();
    static uint64_t GetEntityCounter();
    static uint64_t GetVertexCounter();
    static uint64_t GetLastFrameTimeInNanoseconds();
    static Mode GetMode();
    static const RendererStats& GetStats();
    static framebuffer::FrameBuffer* GetFrameBuffer();
    static Signature GetSignature();
    static bool VisitEntities(std::function<bool(const Entity&)> callback);

    static inline uint64_t
    GetTotalTimeSinceLastFrameInNanoseconds(const uint64_t ts = uv_hrtime()) {
      return (ts - GetLastFrameTimeInNanoseconds());
    }

#define DEFINE_STATE_CHECK(Name) \
    static inline bool Is##Name() { return GetState() == RendererState::k##Name##State; }
    
    FOR_EACH_RENDERER_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK
  };
}

#endif //MCC_RENDERER_H