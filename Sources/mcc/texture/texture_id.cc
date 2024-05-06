#include "mcc/texture/texture_id.h"

namespace mcc::texture {
  void GenerateTextureIds(TextureId* ids, const int num) {
    glGenTextures(num, &ids[0]);
    CHECK_GL(FATAL);
  }

  rx::observable<TextureId> GenerateTextureIdsAsync(const int num) {
    return rx::observable<>::create<TextureId>([num](rx::subscriber<TextureId> s) {
      TextureId ids[num];
      GenerateTextureIds(&ids[0], num);
      for(auto idx = 0; idx < num; idx++)
        s.on_next(ids[idx]);
      s.on_completed();
    });
  }
}