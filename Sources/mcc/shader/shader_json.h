#ifndef MCC_SHADER_JSON_H
#define MCC_SHADER_JSON_H

#include <string>
#include <optional>

#include "mcc/json.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/shader_loader.h"

namespace mcc::shader {
  bool IsValidShaderDocument(const json::Document& doc);
  
  static inline bool
  IsValidShaderDocument(const std::string& filename) {
    json::Document doc;
    if(!json::ParseJson(filename, doc))
      return false;
    return IsValidShaderDocument(doc);
  }

  class ShaderDocument {
    static constexpr const auto kVertexProperty = "vertex";
    static constexpr const auto kFragmentProperty = "fragment";
    static constexpr const auto kGeometryProperty = "geometry";
    static constexpr const auto kTessEvalProperty = "tessEval";
    static constexpr const auto kTessControlProperty = "tessControl";
    static constexpr const auto kNameProperty = "name";
    static constexpr const auto kDescriptionProperty = "description";
  protected:
    std::string root_;
    std::string name_;
    json::Document doc_;

    inline std::optional<std::string>
    GetStringProperty(const char* name) const {
      if(!doc_.HasMember(name))
        return std::nullopt;
      const auto& property = doc_[name];
      if(!property.IsString())
        return std::nullopt;
      const auto value = std::string(property.GetString(), property.GetStringLength());
      return std::optional<std::string>{ value };
    }

    inline bool
    IsBoolProperty(const char* name) const {
      if(!doc_.HasMember(name))
        return false;
      return doc_[name].IsBool()
          && doc_[name].GetBool();
    }

    static inline std::string
    GetNameFromFilename(const std::string& filename) {
      auto name = filename;
      const auto slashpos = name.find_last_of('/');
      if(slashpos != std::string::npos)
        name = name.substr(slashpos + 1);
      const auto dotpos = name.find_last_of('.');
      if(dotpos != std::string::npos)
        name = name.substr(0, dotpos);
      return name;
    }

    static inline std::string
    GetDirectoryOf(const std::string& filename) {
      auto name = filename;
      const auto dotpos = name.find_last_of('.');
      if(dotpos == std::string::npos) 
        return name;
      const auto slashpos = name.find_last_of('/');
      if(slashpos != std::string::npos)
        name = name.substr(0, slashpos);
      return name;
    }

    inline std::string GetDefaultVertexShaderFilename() const {
      return root_ + "/" + name_ + ".vs";
    }

    inline std::string GetDefaultFragmentShaderFilename() const {
      return root_ + "/" + name_ + ".fs";
    }

    inline std::string GetDefaultGeometryShaderFilename() const {
      return root_ + "/" + name_ + ".gs";
    }

    inline std::string GetDefaultTessEvalShaderFilename() const {
      return root_ + "/" + name_ + ".tes";
    }

    inline std::string GetDefaultTessControlShaderFilename() const {
      return root_ + "/" + name_ + ".tcs";
    }
  public:
    ShaderDocument(const std::string& name, const std::string& filename):
      doc_(),
      name_(name),
      root_(GetDirectoryOf(filename)) {
      if(!json::ParseJson(filename, doc_)) {
        DLOG(ERROR) << "failed to parse shader document from: " << filename;
        return;
      }
      if(!IsValid()) {
        DLOG(ERROR) << "parsed invalid shader document from: " << filename;
        return;
      }
      const auto pName = GetStringProperty("name");
      if(!pName) {
        name_ = (*pName);
        DLOG(INFO) << "set " << name << " shader's name to " << name_;
      }
      DLOG(INFO) << "parsed " << name_ << " shader doc in " << root_;
    }
    explicit ShaderDocument(const std::string& filename):
      ShaderDocument(GetNameFromFilename(filename), filename) {
    }
    virtual ~ShaderDocument() = default;

    std::string GetVertexShader() const {
      MCC_ASSERT(doc_.HasMember(kVertexProperty));
      if(IsBoolProperty(kVertexProperty))
        return GetDefaultVertexShaderFilename();
      const auto path = GetStringProperty(kVertexProperty);
      if(!path || (*path).empty())
        return GetDefaultVertexShaderFilename();
      auto filename = (*path);
      if(!StartsWith(filename, "/"))
        filename = "/" + filename; 
      if(!StartsWith(filename, root_))
        filename = root_ + "/" + filename;
      //TODO: check extension
      return filename;
    }

    std::string GetFragmentShader() const {
      MCC_ASSERT(doc_.HasMember(kFragmentProperty));
      if(IsBoolProperty(kFragmentProperty))
        return GetDefaultFragmentShaderFilename();
      const auto path = GetStringProperty(kFragmentProperty);
      if(!path || (*path).empty())
        return GetDefaultFragmentShaderFilename();
      auto filename = (*path);
      if(!StartsWith(filename, "/"))
        filename = "/" + filename; 
      if(!StartsWith(filename, root_))
        filename = root_ + "/" + filename;
      //TODO: check extension
      return filename;
    }

    std::optional<std::string> GetGeometryShader() const {
      if(!doc_.HasMember(kGeometryProperty))
        return std::nullopt;
      if(IsBoolProperty(kGeometryProperty))
        return GetDefaultGeometryShaderFilename();
      const auto path = GetStringProperty(kGeometryProperty);
      if(!path || (*path).empty())
        return GetDefaultGeometryShaderFilename();
      auto filename = (*path);
      if(!StartsWith(filename, "/"))
        filename = "/" + filename; 
      if(!StartsWith(filename, root_))
        filename = root_ + "/" + filename;
      //TODO: check extension
      return std::optional<std::string>{ filename };
    }

    std::optional<std::string> GetTessEvalShader() const {
      if(!doc_.HasMember(kTessEvalProperty))
        return std::nullopt;
      if(IsBoolProperty(kTessEvalProperty))
        return GetDefaultTessEvalShaderFilename();
      const auto path = GetStringProperty(kTessEvalProperty);
      if(!path || (*path).empty())
        return GetDefaultTessEvalShaderFilename();
      auto filename = (*path);
      if(!StartsWith(filename, "/"))
        filename = "/" + filename; 
      if(!StartsWith(filename, root_))
        filename = root_ + "/" + filename;
      //TODO: check extension
      return std::optional<std::string>{ filename };
    }

    std::optional<std::string> GetTessControlShader() const {
      if(!doc_.HasMember(kTessControlProperty))
        return std::nullopt;
      if(IsBoolProperty(kTessControlProperty))
        return GetDefaultTessControlShaderFilename();
      const auto path = GetStringProperty(kTessControlProperty);
      if(!path || (*path).empty())
        return GetDefaultTessControlShaderFilename();
      auto filename = (*path);
      if(!StartsWith(filename, "/"))
        filename = "/" + filename; 
      if(!StartsWith(filename, root_))
        filename = root_ + "/" + filename;
      //TODO: check extension
      return std::optional<std::string>{ filename };
    }

    virtual std::string GetIssue() const {
      if(doc_.HasParseError())
        return json::GetParseError(doc_);
      return "Document schema is invalid."; //TODO: more
    }

    virtual bool IsValid() const {
      return !doc_.HasParseError() && IsValidShaderDocument(doc_);
    }
  };
}

#endif //MCC_SHADER_JSON_H