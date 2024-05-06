#include "mcc/texture/cube_map_loader_json.h"
#include "mcc/json_spec.h"
#include "mcc/texture/texture.h"
#include "mcc/texture/cube_map_builder.h"

namespace mcc {
  namespace json {
    class CubeMapFaceValue {
      static constexpr const auto kBorderPropertyName = "border";
      static constexpr const auto kLevelPropertyName = "level";
      static constexpr const auto kFilePropertyName = "file";
    protected:
      texture::CubeMapFace face_;
      const Value& value_;

      inline std::optional<const Value*> GetProperty(const char* name) const {
        if(!IsObject() || !value_.HasMember(name))
          return std::nullopt;
        return { &value_[name] };
      }
    public:
      explicit CubeMapFaceValue(const texture::CubeMapFace face, const Value& value):
        face_(face),
        value_(value) {
        MCC_ASSERT(value.IsString() || value.IsObject());
      }
      explicit CubeMapFaceValue(const texture::CubeMapFace face, const Value* value):
        CubeMapFaceValue(face, *value) {
      }
      ~CubeMapFaceValue() = default;

      texture::CubeMapFace GetFace() const {
        return face_;
      }

      bool IsObject() const {
        return value_.IsObject();
      }

      bool IsString() const {
        return value_.IsString();
      }

      std::optional<const Value*> GetBorderProperty() const {
        return GetProperty(kBorderPropertyName);
      }

      int GetBorder() const {
        const auto border = GetBorderProperty();
        return border
             ? (*border)->GetInt()
             : 0;
      }

      std::optional<const Value*> GetLevelProperty() const {
        return GetProperty(kLevelPropertyName);
      }

      int GetLevel() const {
        const auto level = GetLevelProperty();
        return level
             ? (*level)->GetInt()
             : 0;
      }

      std::optional<const Value*> GetFileProperty() const {
        return GetProperty(kFilePropertyName);
      }

      std::string GetFilePath() const {
        if(IsString())
          return std::string(value_.GetString(), value_.GetStringLength());
        const auto file = GetFileProperty();
        MCC_ASSERT(file);
        MCC_ASSERT((*file)->IsString());
        return std::string((*file)->GetString(), (*file)->GetStringLength());
      }
    };
  }
}

namespace mcc::texture {
  CubeMap* JsonCubeMapLoader::LoadTexture() const {
    DLOG(INFO) << "loading CubeMap from json: " << GetUri();
    json::Document doc;
    if(!json::ParseJson(GetUri(), doc)) {
      LOG(ERROR) << "failed to parse Texture2d json from: " << GetUri();
      return nullptr;
    }

    MCC_ASSERT(doc.IsObject());
    json::SpecDocument spec(doc.GetObject());
    const auto name = spec.GetName();
    const auto type = spec.GetType();
    MCC_ASSERT(EqualsIgnoreCase(type, "cubemap"));
    MCC_ASSERT(spec.HasSpecProperty());
    const auto& spec_prop = spec.GetSpecProperty();
    MCC_ASSERT(spec_prop.IsObject());
    const auto spec_prop_obj = spec_prop.GetObject();

    CubeMapBuilder builder;
    json::ConstCubeMapObject cube_map(spec_prop_obj);
    const auto top = cube_map.GetTopProperty();
    if(top) {
      json::CubeMapFaceValue value(kTop, *top);
      auto path = value.GetFilePath();
      if(!StartsWith(path, dir_))
        path = fmt::format("{0:s}/{1:s}", dir_, path);
      const auto img = img::Decode(uri::Uri(fmt::format("file://{0:s}", path)));
      MCC_ASSERT(img);
      builder.SetTop(img, value.GetLevel(), value.GetBorder());
    }

    const auto bottom = cube_map.GetBottomProperty();
    if(bottom) {
      json::CubeMapFaceValue value(kBottom, *top);
      auto path = value.GetFilePath();
      if(!StartsWith(path, dir_))
        path = fmt::format("{0:s}/{1:s}", dir_, path);
      const auto img = img::Decode(uri::Uri(fmt::format("file://{0:s}", path)));
      MCC_ASSERT(img);
      builder.SetBottom(img, value.GetLevel(), value.GetBorder());
    }

    const auto front = cube_map.GetFrontProperty();
    if(front) {
      json::CubeMapFaceValue value(kFront, *top);
      auto path = value.GetFilePath();
      if(!StartsWith(path, dir_))
        path = fmt::format("{0:s}/{1:s}", dir_, path);
      const auto img = img::Decode(uri::Uri(fmt::format("file://{0:s}", path)));
      MCC_ASSERT(img);
      builder.SetFront(img, value.GetLevel(), value.GetBorder());
    }

    const auto back = cube_map.GetBackProperty();
    if(back) {
      json::CubeMapFaceValue value(kBack, *top);
      auto path = value.GetFilePath();
      if(!StartsWith(path, dir_))
        path = fmt::format("{0:s}/{1:s}", dir_, path);
      const auto img = img::Decode(uri::Uri(fmt::format("file://{0:s}", path)));
      MCC_ASSERT(img);
      builder.SetBack(img, value.GetLevel(), value.GetBorder());
    }

    const auto left = cube_map.GetLeftProperty();
    if(left) {
      json::CubeMapFaceValue value(kLeft, *top);
      auto path = value.GetFilePath();
      if(!StartsWith(path, dir_))
        path = fmt::format("{0:s}/{1:s}", dir_, path);
      const auto img = img::Decode(uri::Uri(fmt::format("file://{0:s}", path)));
      MCC_ASSERT(img);
      builder.SetLeft(img, value.GetLevel(), value.GetBorder());
    }

    const auto right = cube_map.GetRightProperty();
    if(right) {
      json::CubeMapFaceValue value(kRight, *top);
      auto path = value.GetFilePath();
      if(!StartsWith(path, dir_))
        path = fmt::format("{0:s}/{1:s}", dir_, path);
      const auto img = img::Decode(uri::Uri(fmt::format("file://{0:s}", path)));
      MCC_ASSERT(img);
      builder.SetRight(img, value.GetLevel(), value.GetBorder());
    }
    return CubeMap::New(builder);
  }
}