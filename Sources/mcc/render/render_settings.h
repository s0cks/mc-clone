#ifndef MCC_RENDER_SETTINGS_H
#define MCC_RENDER_SETTINGS_H

#include "mcc/resolution.h"
#include "mcc/settings/setting.h"
#include "mcc/render/render_flags.h"

namespace mcc::render {
  class ResolutionSetting : public settings::SettingTemplate<Resolution> {
  public:
    explicit ResolutionSetting(const Resolution& init_value):
      SettingTemplate(init_value) {
    }
    ~ResolutionSetting() override = default;
    DECLARE_SETTING(Resolution);
  public:
    static inline ResolutionSetting*
    New(const Resolution& init_value) {
      return new ResolutionSetting(init_value);
    }
  };

  ResolutionSetting* GetResolutionSetting();

  static inline Resolution
  GetResolution() {
    const auto res = GetResolutionSetting();
    MCC_ASSERT(res);
    return res->GetValue();
  }
}

#endif //MCC_RENDER_SETTINGS_H