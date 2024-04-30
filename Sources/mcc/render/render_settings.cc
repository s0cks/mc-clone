#include "mcc/render/render_settings.h"
#include <sstream>

namespace mcc::render {
  static ResolutionSetting* resolution_ = nullptr;

  std::string ResolutionSetting::ToString() const {
    std::stringstream ss;
    ss << "resolution=" << GetValue();
    return ss.str();
  }

  ResolutionSetting* GetResolutionSetting() {
    if(resolution_)
      return resolution_;
    return resolution_ = new ResolutionSetting(GetDefaultResolution());
  }
}