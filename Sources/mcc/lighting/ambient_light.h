#ifndef MCC_AMBIENT_LIGHT_H
#define MCC_AMBIENT_LIGHT_H

#include "mcc/gfx.h"
#include "mcc/component/component.h"
#include "mcc/component/component_state_table.h"

namespace mcc {
  struct AmbientLight {
    glm::vec3 color;
    float intensity;

    DECLARE_COMPONENT(AmbientLight);
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
  public:
    static void Init();
  };
}

#endif //MCC_AMBIENT_LIGHT_H