#include "mcc/shader/shader.h"

#include "mcc/json_schema.h"
#include "mcc/shader/shader_compiler.h"

namespace mcc::shader {
  const std::set<std::string> VertexShader::kValidExtensions = {
    "json",
    "glsl",
    "vert",
  };

  VertexShader* VertexShader::FromSource(ShaderCode* source) {
    MCC_ASSERT(source);
    const auto id = ShaderCompiler::Compile(source);
    if(IsInvalidShaderId(id)) {
      LOG(ERROR) << "failed to compile: " << source->ToString();
      return nullptr;
    }
    return New(id);
  }

  VertexShader* VertexShader::FromSource(const uri::Uri& uri) {
    if(uri.HasScheme("file")) {
      return FromSource(ShaderCode::VertexShaderCodeFromFile(uri));
    } else if(uri.HasScheme("shader")) {
      const auto base_path = fmt::format("{0:s}/shaders/", FLAGS_resources);
      auto path = uri.path;
      if(!StartsWith(path, base_path))
        path = fmt::format("{0:s}/{1:s}", base_path, path[0] == '/' ? path.substr(1) : path);
      if(!EndsWith(path, ".vert"))
        path = fmt::format("{0:s}.vert", path);
      const auto new_uri = uri::Uri(fmt::format("file://{0:s}", path));
      return FromSource(ShaderCode::VertexShaderCodeFromFile(new_uri));
    }
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  VertexShader* VertexShader::FromJson(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension("json"));
    json::Document doc;
    if(!json::ParseJson(uri, doc)) {
      LOG(ERROR) << "failed to parse VertexShader json: " << uri;
      return nullptr;
    }

    const auto schema = json::GetSchema();
    MCC_ASSERT(schema);
    const auto valid = schema->Validate(doc);
    if(!valid) {
      LOG(ERROR) << "VertexShader json is invalid: " << valid;
      return nullptr;
    }

    const auto& name_prop = doc["name"];
    MCC_ASSERT(name_prop.IsString());
    const auto name = std::string(name_prop.GetString(), name_prop.GetStringLength());
    MCC_ASSERT(!name.empty());
    
    const auto& type_prop = doc["type"];
    MCC_ASSERT(type_prop.IsString());
    const auto type = std::string(type_prop.GetString(), type_prop.GetStringLength());
    if(!EqualsIgnoreCase(type, "vertexshader")) {
      LOG(ERROR) << "invalid type " << type << " for VertexShader json.";
      return nullptr;
    }
    const auto& data_prop = doc["data"];
    DLOG(INFO) << "loading " << name << " " << type << " shader from json....";
    return FromJson(data_prop);
  }

  VertexShader* VertexShader::FromJson(const json::Value& value) {
    if(value.IsString()) {
      const auto val = std::string(value.GetString(), value.GetStringLength());
      return FromSource(val);
    } else if(value.IsObject()) {
      const auto unit = ShaderUnit::NewVertexShaderUnit("");

      const auto val = value.GetObject();
      if(val.HasMember("source") && val["source"].IsString()) {
        const auto& source = val["source"];
        unit->Append(std::string(source.GetString(), source.GetStringLength()));
      }

      if(val.HasMember("sources") && val["source"].IsArray()) {
        const auto& sources = val["sources"].GetArray();
        for(const auto& source : sources) {
          MCC_ASSERT(source.IsString());
          unit->Append(std::string(source.GetString(), source.GetStringLength()));
        }
      }

      const auto id = ShaderCompiler::Compile(unit);
      if(IsInvalidShaderId(id)) {
        LOG(ERROR) << "failed to compile: " << unit->ToString();
        return nullptr;
      }
      return New(id);
    }

    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }
  
  VertexShader* VertexShader::FromJson(const char* source, const uword length) {
    if(StartsWith(source, length, "file:")) {
      return FromJson(uri::Uri(source));
    } else if(StartsWith(source, length, "shader:")) {
      const auto uri = uri::Uri(source);
      return FromJson(uri.ToFileUri(fmt::format("{0:s}/shaders", FLAGS_resources)));
    }

    json::Document doc;
    if(!json::ParseRawJson(source, doc)) {
      LOG(ERROR) << "failed to parse VertexShader json: " << source;
      return nullptr;
    }

    const auto schema = json::GetSchema();
    MCC_ASSERT(schema);
    const auto valid = schema->Validate(doc);
    if(!valid) {
      LOG(ERROR) << "VertexShader json is invalid: " << valid;
      return nullptr;
    }

    const auto& name_prop = doc["name"];
    MCC_ASSERT(name_prop.IsString());
    const auto name = std::string(name_prop.GetString(), name_prop.GetStringLength());
    MCC_ASSERT(!name.empty());
    
    const auto& type_prop = doc["type"];
    MCC_ASSERT(type_prop.IsString());
    const auto type = std::string(type_prop.GetString(), type_prop.GetStringLength());
    if(!EqualsIgnoreCase(type, "vertexshader")) {
      LOG(ERROR) << "invalid type " << type << " for VertexShader json.";
      return nullptr;
    }
    const auto& data_prop = doc["data"];
    DLOG(INFO) << "loading " << name << " " << type << " shader from json....";
    return FromJson(data_prop);
  }

  std::string VertexShader::ToString() const {
    std::stringstream ss;
    ss << "VertexShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }
}