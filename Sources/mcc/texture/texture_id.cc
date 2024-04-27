#include "mcc/texture/texture_id.h"

namespace mcc::texture {
  rx::observable<TextureId> GenerateTextureIds(const int num) {
    return rx::observable<>::create<TextureId>([num](rx::subscriber<TextureId> s) {
      TextureId ids[num];
      glGenTextures(num, &ids[0]);
      CHECK_GL(FATAL);

      for(auto idx = 0; idx < num; idx++)
        s.on_next(ids[idx]);
      s.on_completed();
    });
  }
}