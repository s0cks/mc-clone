#ifndef MCC_SHADER_DOCUMENT_H
#define MCC_SHADER_DOCUMENT_H

#include <string>
#include <optional>

#include "mcc/json.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/source.h"
#include "mcc/shader/shader_loader.h"
#include "mcc/shader/shader_constants.h"

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

    virtual std::string GetName() const = 0;
    virtual uri::Uri GetVertexShaderUri() const = 0;
    virtual uri::Uri GetFragmentShaderUri() const = 0;
    virtual std::optional<uri::Uri> GetGeometryShaderUri() = 0;
    virtual std::optional<uri::Uri> GetTessControlShaderUri() = 0;
    virtual std::optional<uri::Uri> GetTessEvalShaderUri() = 0;

    rx::observable<ShaderSource> GetSources() const; 

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

#endif // MCC_SHADER_DOCUMENT_H