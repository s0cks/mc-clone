#include "mcc/texture/texture_loader.h"

#include <regex>
#include "mcc/json.h"

namespace mcc::texture {
  static const std::regex kPngPattern(".*\\.(png)$");
  static const std::regex kJpegPattern(".*\\.(jpeg|jpg)$");
  static const std::regex kJsonPattern(".*\\.(json)$");

  std::optional<Texture*> PngFileLoader::LoadTexture() {
    DLOG(INFO) << "loading png texture from: " << filename_;
    if(!png::Decode(filename_, image_)) {
      LOG(ERROR) << "failed to load " << filename_;
      return std::nullopt;
    }
    // glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    // CHECK_GL(FATAL);
    const auto texture = new Texture(k2D, true, true, true,
                                     GL_RGB, image_.size, 
                                     image_.type, GL_UNSIGNED_BYTE, true,
                                     (const GLvoid*) image_.data->data());
    return std::optional<Texture*>{ texture };
  }

  std::optional<Texture*> JpegFileLoader::LoadTexture() {
    DLOG(INFO) << "loading jpeg texture from: " << filename_;
    if(!jpeg::Decode(filename_, image_)) {
      LOG(ERROR) << "failed to load " << filename_;
      return std::nullopt;
    }
    const auto texture = new Texture(k2D, true, true, true,
                                     image_.type, image_.size, 
                                     image_.type, GL_UNSIGNED_BYTE, true,
                                     (const GLvoid*) image_.data->data());
    return std::optional<Texture*>{ texture };
  }

  TextureRef TextureLoader::Load() {
    DLOG(INFO) << "loading Texture from " << filename_ << "....";
    if(IsDirectory(filename_)) {
      const auto index = filename_ + "/texture.json";
      DLOG(INFO) << "checking for index: " <<  index;
      if(!FileExists(index)) {
        DLOG(ERROR) << "no Texture index found in: " << filename_;
        return TextureRef();
      }

      JsonTextureLoader loader(tag_, index);
      return loader.Load();
    } else if(std::regex_match(filename_, kPngPattern)) {
      PngFileLoader loader(tag_, filename_);
      return loader.Load();
    } else if(std::regex_match(filename_, kJpegPattern)) {
      JpegFileLoader loader(tag_, filename_);
      return loader.Load();
    } else if(std::regex_match(filename_, kJsonPattern)) {
      JsonTextureLoader loader(tag_, filename_);
      return loader.Load();
    }

    return TextureRef();
  }

  static const auto kParseTextureTarget = [](const std::string& target) {
    if(target == "1d") {
      return std::optional<TextureTarget>{ k1D };
    } else if(target == "2d") {
      return std::optional<TextureTarget>{ k2D };
    } else if(target == "3d") {
      return std::optional<TextureTarget>{ k3D };
    }
    DLOG(ERROR) << "unknown target: " << target;
    return std::optional<TextureTarget>{};
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

  class TextureJsonLoader : public res::JsonLoader<Texture> {
  public:
    explicit TextureJsonLoader(const res::Tag& tag,
                               json::Document& doc):
      JsonLoader<Texture>(tag, doc) {
    }
    ~TextureJsonLoader() override = default;

    TextureRef Load() override {
      const auto target = map<std::string, TextureTarget>(GetDocumentString("target"), kParseTextureTarget).value_or(kDefaultTarget);
      const auto texture = GetDocumentString("filename").value_or("");
      if(std::regex_match(texture, kPngPattern)) {
        PngFileLoader loader(tag_, texture);
        return loader.Load();
      } else if(std::regex_match(texture, kJpegPattern)) {
        JpegFileLoader loader(tag_, texture);
        return loader.Load();
      }

      return TextureRef();
    }
  };

  class CubeMapJsonLoader : public res::JsonLoader<Texture> {
  private:
    const std::string& filename_;
    std::string dirname_;

    inline std::optional<std::string>
    GetDefaultFaceTexture() {
      return GetDocumentString("default");
    }

    inline std::optional<std::string>
    GetFaceTexture(const CubeMapFace& face) {
      return GetDocumentString(ToString(face));
    }

    bool LoadCubeMapFace(const CubeMapFace face) {
      auto tex = GetFaceTexture(face);
      if(!tex) {
        DLOG(WARNING) << "couldn't load cube map face " << face << ".";
        tex = GetDefaultFaceTexture();
        if(!tex) {
          DLOG(ERROR) << "couldn't load cube map face " << face << ", no default texture.";
          return false;
        }
      }

      const auto dir = FLAGS_resources + "/textures/" + dirname_;
      const auto filename = (*tex);
      auto full_filename = filename;
      if(!StartsWith(full_filename, dir))
        full_filename = dir + "/" + full_filename;

      DLOG(INFO) << "loading cubemap " << face << " face from: " << filename;
      Image image;
      if(std::regex_match(filename, kJpegPattern)) {
        if(!jpeg::Decode(full_filename, image)) {
          DLOG(ERROR) << "failed to load cube map face " << face << ", couldn't decode image from: " << filename;
          return false;
        }
        glTexImage2D(face, 0, image.type, image.size[0], image.size[1], 0, image.type, GL_UNSIGNED_BYTE, (const GLvoid*) image.data->data());
      } else if(std::regex_match(filename, kPngPattern)) {
        if(!png::Decode(full_filename, image)) {
          DLOG(ERROR) << "failed to load cube map face " << face << ", couldn't decode image from: " << filename;
          return false;
        }
        glTexImage2D(face, 0, GL_RGB, image.size[0], image.size[1], 0, image.type, GL_UNSIGNED_BYTE, (const GLvoid*) image.data->data());
      }
      CHECK_GL(FATAL);
      return true;
    }
  public:
    CubeMapJsonLoader(const res::Tag& tag,
                      json::Document& doc,
                      const std::string& filename,
                      const std::string& dirname):
      JsonLoader<Texture>(tag, doc),
      filename_(filename),
      dirname_(dirname) {    
    }
    ~CubeMapJsonLoader() override = default;

    TextureRef Load() override {
      const auto root = GetDocumentString("root");
      const auto filter = TextureFilter(TextureFilterComponent::kLinear);
      const auto texture = new Texture((const TextureTarget) kCubeMap, true, true, false, filter);
      LOG_IF(FATAL, !LoadCubeMapFace(kRightFace)) << "failed to load cube map right face.";
      LOG_IF(FATAL, !LoadCubeMapFace(kLeftFace)) << "failed to load cube map left face.";
      LOG_IF(FATAL, !LoadCubeMapFace(kTopFace)) << "failed to load cube map top face.";
      LOG_IF(FATAL, !LoadCubeMapFace(kBottomFace)) << "failed to load cube map bottom face.";
      LOG_IF(FATAL, !LoadCubeMapFace(kFrontFace)) << "failed to load cube map front face.";
      LOG_IF(FATAL, !LoadCubeMapFace(kBackFace)) << "failed to load cube map back face.";
      texture->Unbind();
      const auto ptr = resource::Pointer(tag_, (uword) texture);
      return TextureRef(ptr);
    }
  };

  TextureRef JsonTextureLoader::Load() {
    const auto type = GetDocumentType();
    if(!type)
      return TextureRef();
    if((*type) == "cubemap") {
      auto dirname = filename_.substr(0, filename_.find_last_of('/'));
      dirname = dirname.substr(dirname.find_last_of('/') + 1);
      CubeMapJsonLoader loader(tag_, doc_, filename_, dirname);
      return loader.Load();
    } else if((*type) == "texture") {
      TextureJsonLoader loader(tag_, doc_);
      return loader.Load();
    }

    DLOG(ERROR) << "failed to load texture from: " << filename_;
    return TextureRef();
  }
}