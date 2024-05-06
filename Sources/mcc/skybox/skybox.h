#ifndef MCC_SKYBOX_H
#define MCC_SKYBOX_H

#include "mcc/texture/texture.h"
#include "mcc/skybox/skybox_events.h"
#include "mcc/skybox/skybox_builder.h"

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
      CubeMap* texture_;

      Skybox(SkyboxMesh* mesh, CubeMap* texture);
    public:
      virtual ~Skybox();
      
      virtual SkyboxMesh* GetMesh() const {
        return mesh_;
      }

      virtual CubeMap* GetTexture() const {
        return texture_;
      }

      virtual std::string ToString() const;
    public:
      static Skybox* New(const uri::Uri& uri);
      static inline Skybox*
      New(const SkyboxBuilder& builder) {
        return new Skybox(builder.GetMesh(), builder.GetTexture());
      }

      static inline Skybox*
      New(const uri::basic_uri& uri) {
        auto target = uri;
        if(!(StartsWith(target, "skybox://") || StartsWith(target, "texture://")))
          target = fmt::format("skybox://{0:s}");
        return New(uri::Uri(target));
      }
    };

    Skybox* GetCurrentSkybox();
    void SetCurrentSkybox(Skybox* value);
  }
  using skybox::Skybox;
}

#endif //MCC_SKYBOX_H