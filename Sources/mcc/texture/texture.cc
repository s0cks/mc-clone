#include <regex>
#include <algorithm>
#include <functional>
#include <unordered_set>

#include "mcc/uri.h"
#include "mcc/flags.h"
#include "mcc/common.h"
#include "mcc/image/image.h"
#include "mcc/texture/texture.h"
#include "mcc/texture/texture_spec.h"

#include "mcc/texture/texture_factory.h"

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

  TextureRef Texture2D::New(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("tex") || uri.HasScheme("texture"));
    const auto extension = uri.GetPathExtension();
    if(!extension) {
      //TODO: resolve w/o extension
      return {};
    }

    if(EqualsIgnoreCase(*extension, "json")) {
    } else if(EqualsIgnoreCase(*extension, "png")) {
      const auto abs_path = uri::Uri(fmt::format("file://{0:s}/textures/{1:s}", FLAGS_resources, uri.path));
      if(!FileExists(abs_path)) {
        DLOG(ERROR) << "no texture found at: " << abs_path;
        return {};
      }

      const auto image = img::png::Decode(abs_path);
      if(!image) {
        DLOG(ERROR) << "failed to decode png from: " << abs_path;
        return {};
      }

      TextureSpecBuilder<k2D> builder;
      const auto spec = builder.Build();
      TextureFactory2D factory(spec);
      return Texture2D::New(factory.Create(image));
    } else if(EqualsIgnoreCase(*extension, "jpeg")
           || EqualsIgnoreCase(*extension, "jpg")) {
    }

    LOG(ERROR) << "unknown texture extension: " << *extension;
    return {};
  }
  
  TextureRef Texture3D::New(const uri::Uri& uri) {
    NOT_IMPLEMENTED(FATAL);
    return {};
  }

  TextureRef CubeMap::New(const uri::Uri& uri) {
    NOT_IMPLEMENTED(FATAL);
    return {};
  }
}