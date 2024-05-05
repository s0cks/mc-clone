#include "mcc/skybox/skybox_events.h"
#include <sstream>
#include "mcc/skybox/skybox.h"

namespace mcc::skybox {
  std::string SkyboxCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "SkyboxCreatedEvent(";
    ss << "skybox=" << GetSkybox()->ToString();
    ss << ")";
    return ss.str();
  }

  std::string SkyboxDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "SkyboxDestroyedEvent(";
    ss << "skybox=" << GetSkybox()->ToString();
    ss << ")";
    return ss.str();
  }

  std::string SkyboxTextureChangedEvent::ToString() const {
    std::stringstream ss;
    ss << "TextureChangedEvent(";
    ss << "skybox=" << GetSkybox()->ToString();
    ss << ")";
    return ss.str();
  }
}