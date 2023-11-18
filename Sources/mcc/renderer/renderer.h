#ifndef MCC_RENDERER_H
#define MCC_RENDERER_H

#include "mcc/common.h"
#include "mcc/mesh/mesh.h"
#include "mcc/state_machine.h"
#include "mcc/renderer/renderable.h"
#include "mcc/renderer/renderer_state.h"
#include "mcc/renderer/renderer_stats.h"
#include "mcc/engine/tick.h"
#include "mcc/ecs/entity.h"
#include "mcc/framebuffer.h"

#include "mcc/skybox.h"
#include "mcc/camera/perspective_camera.h"

namespace mcc {
  namespace gui {
    class Screen;
    class Frame;
    class SettingsFrame;
    class RendererFrame;
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

    friend class PreRenderStage;
    friend class RenderStage;
    friend class PostRenderStage;
    friend class RenderTerrainStage;
    friend class RenderEntitiesStage;
    friend class RenderEntityPipeline;
    friend class RendererPipeline;
    friend class mcc::terrain::RenderTerrainChunkPipeline;
    DECLARE_STATE_MACHINE(Renderer);
  public:
    enum Mode {
      kFillMode = GL_FILL,
      kWireframeMode = GL_LINE,

      kNumberOfModes,
      kDefaultMode = kFillMode,
    };
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
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
  public:
    static void Init();
    static void Run(const uv_run_mode = UV_RUN_NOWAIT); //TODO: make private
    static uint64_t GetFrameCount();
    static uint64_t GetFPS();
    static uint64_t GetEntityCounter();
    static uint64_t GetVertexCounter();
    static uint64_t GetLastFrameTimeInNanoseconds();
    static Mode GetMode();
    static uv_loop_t* GetLoop();
    static RendererSampleSeries* GetSamples();
    static FrameBuffer* GetFrameBuffer();
    static Signature GetSignature();
    static bool VisitEntities(std::function<bool(const Entity&)> callback);
    static camera::PerspectiveCameraDataUniformBufferObject* GetCameraUniformBuffer();
    static skybox::Skybox* GetSkybox();

#define DEFINE_STATE_CHECK(Name) \
    static inline bool Is##Name() { return GetState() == RendererState::k##Name##State; }
    
    FOR_EACH_RENDERER_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK
  };
}

#endif //MCC_RENDERER_H