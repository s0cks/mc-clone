#include "mcc/skybox/skybox.h"
#include <sstream>
#include "mcc/thread_local.h"
#include "mcc/skybox/skybox_mesh.h"

namespace mcc::skybox {
  rx::subject<SkyboxEvent*> events_;

  rx::observable<SkyboxEvent*> OnSkyboxEvent() {
    return events_.get_observable();
  }

  Skybox::Skybox(SkyboxMesh* mesh,
                 CubeMap* texture):
    mesh_(mesh),
    texture_(texture) {
    MCC_ASSERT(mesh);
    MCC_ASSERT(texture);
    Publish<SkyboxCreatedEvent>(this);
  }

  Skybox::~Skybox() {
    Publish<SkyboxDestroyedEvent>(this);
  }

  void Skybox::Publish(SkyboxEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    return subscriber.on_next(event);
  }

  std::string Skybox::ToString() const {
    std::stringstream ss;
    ss << "Skybox(";
    ss << ")";
    return ss.str();
  }

  static inline Skybox*
  NewFromTextureUri(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("texture"));
    SkyboxBuilder builder;
    builder.SetTexture(uri);
    return builder.Build();
  }

  Skybox* Skybox::New(const uri::Uri& uri) {
    DLOG(INFO) << "creating new Skybox from: " << uri;
    if(uri.HasScheme("texture"))
      return NewFromTextureUri(uri);
    MCC_ASSERT(uri.HasScheme("skybox"));
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  static ThreadLocal<Skybox> skybox_;

  static inline void
  Publish(SkyboxEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  template<class E, typename... Args>
  static inline void
  Publish(Args... args) {
    E event(args...);
    return Publish((SkyboxEvent*) &event);
  }

  void SetCurrentSkybox(Skybox* value) {
    MCC_ASSERT(value);
    const auto old = skybox_.Get();
    skybox_.Set(value);
    Publish<SkyboxChangedEvent>(value, old);
  }

  Skybox* GetCurrentSkybox() {
    return skybox_.Get();
  }
}