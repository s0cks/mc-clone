#ifndef MCC_RENDERER_H
#define MCC_RENDERER_H

#include "mcc/common.h"
#include "mcc/mesh/mesh.h"
#include "mcc/state_machine.h"
#include "mcc/ecs/system.h"
#include "mcc/renderer/renderable.h"
#include "mcc/renderer/renderer_state.h"

namespace mcc::gui {
  class Screen;
  class Frame;
  class SettingsFrame;
}

namespace mcc::renderer {
  class Renderer {
    friend class gui::Screen;
    friend class gui::SettingsFrame;
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
    static void OnTick(const Tick& tick);
    static void RenderTerrain(const glm::mat4 projection, const glm::mat4 view);
    static void RenderEntity(const glm::mat4 projection, const glm::mat4 view, const Entity e);

    static void PreRender();
    static void PostRender();

    static void SetMode(const Mode mode);

    static inline void
    ResetMode() {
      return SetMode(kDefaultMode);
    }
  public:
    static void Init();
    static uint64_t GetFrameCount();
    static uint64_t GetFPS();

    static void AddFrame(std::shared_ptr<gui::Frame> frame);

    static Mode GetMode();

#define DEFINE_STATE_CHECK(Name) \
    static inline bool Is##Name() { return GetState() == RendererState::k##Name##State; }
    
    FOR_EACH_RENDERER_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK
  };
}

#endif //MCC_RENDERER_H