#ifndef MCC_RENDERER_STATE_H
#define MCC_RENDERER_STATE_H

namespace mcc::renderer {
#define FOR_EACH_RENDERER_STATE(V) \
  V(PreInit)                       \
  V(Init)                          \
  V(PostInit)                      \
  V(PreRender)                     \
  V(RenderTerrain)                 \
  V(RenderEntities)                \
  V(RenderGui)                     \
  V(DrawGui)                       \
  V(PostRender)

  enum RendererState {
#define DEFINE_STATE(Name) k##Name##State,
    FOR_EACH_RENDERER_STATE(DEFINE_STATE)
#undef DEFINE_STATE
    kNumberOfStates,
  };

  static inline const char*
  GetStateName(const RendererState state) {
    switch(state) {
#define DEFINE_TOSTRING(Name) \
      case k##Name##State: return #Name "State";
      FOR_EACH_RENDERER_STATE(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      default: return "Unknown State";
    }
  }
}

#endif //MCC_RENDERER_STATE_H