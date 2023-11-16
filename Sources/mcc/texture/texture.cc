#include "mcc/texture/texture.h"
#include <regex>

#include "mcc/flags.h"
#include "mcc/texture/image.h"
#include "mcc/texture/texture_loader.h"

namespace mcc {
  static const std::regex kPngPattern(".*\\.(png)$");
  static const std::regex kJpegPattern(".*\\.(jpeg|jpg)$");
  static const std::regex kJsonPattern(".*\\.(json)$");

  TextureRef GetTexture(const resource::Token& token) {
    const auto& tag = token.tag;
    MCC_ASSERT(tag.type() == resource::kTextureType);
    const auto& filename = token.location;
    texture::TextureLoader loader(tag, filename);
    return loader.Load();
  }
}

namespace mcc::texture {
  static inline bool
  BeginsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() 
        && str.compare(0, prefix.size(), prefix) == 0;
  }

  // Texture Texture::LoadCubeMapFrom(const std::string& filename) {
    // DLOG(INFO) << "loading cubemap from: " << filename;
    // Texture texture(true);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id());
    // CHECK_GL(FATAL);
    // {
    //   const auto face_filename = GetCubeMapFaceImageFilename(filename, kRightFace);
    //   if(!LoadCubeMapFaceFrom(kRightFace, face_filename)) {
    //     LOG(ERROR) << "failed to load right cube map face from: " << face_filename;
    //     return kInvalidTextureId;
    //   }
    // }
    // {
    //   const auto face_filename = GetCubeMapFaceImageFilename(filename, kLeftFace);
    //   if(!LoadCubeMapFaceFrom(kLeftFace, face_filename)) {
    //     LOG(ERROR) << "failed to load left cube map face from: " << face_filename;
    //     return kInvalidTextureId;
    //   }
    // }
    // {
    //   const auto face_filename = GetCubeMapFaceImageFilename(filename, kTopFace);
    //   if(!LoadCubeMapFaceFrom(kTopFace, face_filename)) {
    //     LOG(ERROR) << "failed to load top cube map face from: " << face_filename;
    //     return kInvalidTextureId;
    //   }
    // }
    // {
    //   const auto face_filename = GetCubeMapFaceImageFilename(filename, kBottomFace);
    //   if(!LoadCubeMapFaceFrom(kBottomFace, face_filename)) {
    //     LOG(ERROR) << "failed to load bottom cube map face from: " << face_filename;
    //     return kInvalidTextureId;
    //   }
    // }
    // {
    //   const auto face_filename = GetCubeMapFaceImageFilename(filename, kBackFace);
    //   if(!LoadCubeMapFaceFrom(kBackFace, face_filename)) {
    //     LOG(ERROR) << "failed to load back cube map face from: " << face_filename;
    //     return kInvalidTextureId;
    //   }
    // }
    // {
    //   const auto face_filename = GetCubeMapFaceImageFilename(filename, kFrontFace);
    //   if(!LoadCubeMapFaceFrom(kFrontFace, face_filename)) {
    //     LOG(ERROR) << "failed to load front cube map face from: " << face_filename;
    //     return kInvalidTextureId;
    //   }
    // }

    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // CHECK_GL(FATAL);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // CHECK_GL(FATAL);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // CHECK_GL(FATAL);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // CHECK_GL(FATAL);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // CHECK_GL(FATAL);
    // const auto texture = new texture::TextureTemplate(loader.Load());
    // const auto ptr = resource::Pointer(token.tag, (uword) texture);
    // return TextureRef(ptr);
  // }
}