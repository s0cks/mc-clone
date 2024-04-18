// #include "mcc/camera/perspective_camera_component.h"

// #include "mcc/thread_local.h"

// namespace mcc::camera {
//   static ThreadLocal<PerspectiveCameraComponent> instance_;
  
//   static inline void
//   Set(PerspectiveCameraComponent* component) {
//     MCC_ASSERT(component);
//     instance_.Set(component);
//   }

//   void PerspectiveCameraComponent::Init() {
//     DLOG(INFO) << "initializing....";
//     Set(new PerspectiveCameraComponent());
//   }

//   PerspectiveCameraComponent* PerspectiveCameraComponent::Get() {
//     return instance_.Get();
//   }
// }