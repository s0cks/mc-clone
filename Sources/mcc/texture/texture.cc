#include <regex>
#include <algorithm>
#include <functional>
#include <unordered_set>

#include "mcc/uri.h"
#include "mcc/flags.h"
#include "mcc/common.h"
#include "mcc/image/image.h"
#include "mcc/texture/texture.h"

namespace mcc::texture {
  static rx::subject<TextureEvent*> events_;

  rx::observable<TextureEvent*> OnTextureEvent() {
    return events_.get_observable();
  }

  void Texture::Publish(TextureEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  void Texture::BindTexture(const TextureTarget target, const TextureId id) {
    glBindTexture(target, id);
    CHECK_GL(FATAL);
  }

  void Texture::DeleteTextures(const TextureId* ids, const uint64_t num_ids) {
    glDeleteTextures(num_ids, ids);
    CHECK_GL(FATAL);
  }

  void Texture::ActiveTexture(const int32_t slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    CHECK_GL(FATAL);
  }

  rx::observable<TextureId> Texture::GenerateTextureId(const int num) {
    MCC_ASSERT(num >= 1);
    return rx::observable<>::create<TextureId>([num](rx::subscriber<TextureId> s) {
      TextureId ids[num];
      glGenTextures(num, ids);
      CHECK_GL(FATAL);

      for(auto idx = 0; idx < num; idx++)
        s.on_next(ids[idx]);
      s.on_completed();
    });
  }

  TextureWrap Texture::GetTextureWrap() const {
    Bind(0);
    const auto wrap = TextureWrap::GetTextureWrap<k2D>();
    Unbind();
    return wrap;
  }

  void Texture::Destroy() {
    glDeleteTextures(1, &id_);
    CHECK_GL(FATAL);
  }
}