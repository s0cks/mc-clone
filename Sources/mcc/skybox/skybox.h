#ifndef MCC_SKYBOX_H
#define MCC_SKYBOX_H

#include "mcc/skybox/skybox_events.h"

namespace mcc {
  namespace skybox {
    rx::observable<SkyboxEvent*> OnSkyboxEvent();
#define DEFINE_ON_SKYBOX_EVENT(Name)                    \
    static inline rx::observable<Name##Event*>          \
    On##Name##Event() {                                 \
      return OnSkyboxEvent()                            \
        .filter(Name##Event::Filter)                    \
        .map(Name##Event::Cast);                        \
    }
    FOR_EACH_SKYBOX_EVENT(DEFINE_ON_SKYBOX_EVENT)
#undef DEFINE_ON_SKYBOX_EVENT

    class SkyboxMesh;
    class Skybox {
    private:
      static void Publish(SkyboxEvent* event);

      template<class E, typename... Args>
      static inline void
      Publish(Args... args) {
        E event(args...);
        return Publish((SkyboxEvent*) &event);
      }
    protected:
      SkyboxMesh* mesh_;
    public:
      virtual ~Skybox() = default;
      virtual SkyboxMesh* GetMesh();
      virtual std::string ToString() const;
    };
  }
  using skybox::Skybox;
}

#endif //MCC_SKYBOX_H