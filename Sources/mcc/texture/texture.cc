#include "mcc/texture/texture.h"
#include <regex>

#include "mcc/flags.h"
#include "mcc/texture/image.h"
#include "mcc/texture/texture_loader.h"

namespace mcc {
  static const std::regex kPngPattern(".*\\.png$");
  static const std::regex kJpegPattern(".*\\.(jpeg|jpg)$");

  TextureRef GetTexture(const resource::Token& token) {
    MCC_ASSERT(token.tag.type() == resource::kTextureType);
    const auto& filename = token.location;
    if(std::regex_match(filename, kPngPattern)) {
      texture::PngTextureLoader loader(filename);
      const auto texture = new Texture(loader.Load());
      const auto ptr = resource::Pointer(token.tag, (uword) texture);
      return TextureRef(ptr);
    } else if(std::regex_match(filename, kJpegPattern)) {
      texture::JpegTextureLoader loader(filename);
      const auto texture = new Texture(loader.Load());
      const auto ptr = resource::Pointer(token.tag, (uword) texture);
      return TextureRef(ptr);
    }
    return TextureRef();
  }
}

namespace mcc::texture {
  static inline bool
  BeginsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() 
        && str.compare(0, prefix.size(), prefix) == 0;
  }

  Texture Texture::LoadFrom(std::string filename) {
    if(!BeginsWith(filename, FLAGS_resources))
      filename = FLAGS_resources;

    if(std::regex_match(filename, kPngPattern)) {
      PngTextureLoader loader(filename);
      return loader.Load();
    } else if(std::regex_match(filename, kJpegPattern)) {
      JpegTextureLoader loader(filename);
      return loader.Load();
    }

    return Texture(kInvalidTextureId);
  }

  enum CubeMapFace {
    kRightFace  = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    kLeftFace   = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    kTopFace    = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    kBottomFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    kBackFace   = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    kFrontFace  = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    kNumberOfCubeMapFaces,
  };

  static inline std::string
  GetCubeMapFaceImageFilename(const std::string& filename, const CubeMapFace face) {
    switch(face) {
      case kRightFace:
        return filename + "/right.jpg";
      case kLeftFace:
        return filename + "/left.jpg";
      case kTopFace:
        return filename + "/top.jpg";
      case kBottomFace:
        return filename + "/bottom.jpg";
      case kBackFace:
        return filename + "/back.jpg";
      case kFrontFace:
        return filename + "/front.jpg";
      default:
        return filename;
    }
  }

  static inline bool
  LoadCubeMapFaceFrom(const CubeMapFace face, const std::string& filename) {
    DLOG(INFO) << "loading cubemap " << face << " from: " << filename;
    Image image;
    if(!jpeg::Decode(filename, image))
      return false;
    glTexImage2D(face, 0, image.type, image.size[0], image.size[1], 0, image.type, GL_UNSIGNED_BYTE, (const GLvoid*) image.data->data());
    CHECK_GL(FATAL);
    return true;
  }

  Texture Texture::LoadCubeMapFrom(const std::string& filename) {
    DLOG(INFO) << "loading cubemap from: " << filename;
    Texture texture(true);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id());
    CHECK_GL(FATAL);
    {
      const auto face_filename = GetCubeMapFaceImageFilename(filename, kRightFace);
      if(!LoadCubeMapFaceFrom(kRightFace, face_filename)) {
        LOG(ERROR) << "failed to load right cube map face from: " << face_filename;
        return kInvalidTextureId;
      }
    }
    {
      const auto face_filename = GetCubeMapFaceImageFilename(filename, kLeftFace);
      if(!LoadCubeMapFaceFrom(kLeftFace, face_filename)) {
        LOG(ERROR) << "failed to load left cube map face from: " << face_filename;
        return kInvalidTextureId;
      }
    }
    {
      const auto face_filename = GetCubeMapFaceImageFilename(filename, kTopFace);
      if(!LoadCubeMapFaceFrom(kTopFace, face_filename)) {
        LOG(ERROR) << "failed to load top cube map face from: " << face_filename;
        return kInvalidTextureId;
      }
    }
    {
      const auto face_filename = GetCubeMapFaceImageFilename(filename, kBottomFace);
      if(!LoadCubeMapFaceFrom(kBottomFace, face_filename)) {
        LOG(ERROR) << "failed to load bottom cube map face from: " << face_filename;
        return kInvalidTextureId;
      }
    }
    {
      const auto face_filename = GetCubeMapFaceImageFilename(filename, kBackFace);
      if(!LoadCubeMapFaceFrom(kBackFace, face_filename)) {
        LOG(ERROR) << "failed to load back cube map face from: " << face_filename;
        return kInvalidTextureId;
      }
    }
    {
      const auto face_filename = GetCubeMapFaceImageFilename(filename, kFrontFace);
      if(!LoadCubeMapFaceFrom(kFrontFace, face_filename)) {
        LOG(ERROR) << "failed to load front cube map face from: " << face_filename;
        return kInvalidTextureId;
      }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    CHECK_GL(FATAL);
    return texture;
  }
}