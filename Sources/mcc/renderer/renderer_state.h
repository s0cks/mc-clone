#ifndef MCC_RENDERER_STATE_H
#define MCC_RENDERER_STATE_H

namespace mcc::renderer {
#define FOR_EACH_RENDERER_STATE(V) \
  V(PreInit)                       \
  V(Init)                          \
  V(PostInit)                      \
  V(PreRender)                     \
  V(PostRender)

  enum RendererState {
#define DEFINE_STATE(Name) k##Name##State,
    FOR_EACH_RENDERER_STATE(DEFINE_STATE)
#undef DEFINE_STATE
    kNumberOfStates,
  };
}

#endif //MCC_RENDERER_STATE_H