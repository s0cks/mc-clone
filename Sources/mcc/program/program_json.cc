#include "mcc/program/program_json.h"
#include <glog/logging.h>

namespace mcc::program {
  bool ProgramReaderHandler::String(const char* value, const rapidjson::SizeType length, const bool b) {
    DLOG(INFO) << __FUNCTION__ << "(); State=" << GetState();
    switch(GetState()) {
      case kVertexShader: {
        uri::Uri uri;
        if(!uri::TryParseUri(uri, value, "shader")) {
          DLOG(ERROR) << "failed to parse fragment shader uri: " << value;
          return TransitionTo(kError);
        }
        return OnParseProgramShaderRef(shader::kVertexShader, uri);
      }
      case kFragmentShader: {
        uri::Uri uri;
        if(!uri::TryParseUri(uri, value, "shader")) {
          DLOG(ERROR) << "failed to parse fragment shader uri: " << value;
          return TransitionTo(kError);
        }
        return OnParseProgramShaderRef(shader::kFragmentShader, uri);
      }
      case kGeometryShader: {
        uri::Uri uri;
        if(!uri::TryParseUri(uri, value, "shader")) {
          DLOG(ERROR) << "failed to parse fragment shader uri: " << value;
          return TransitionTo(kError);
        }
        return OnParseProgramShaderRef(shader::kGeometryShader, uri);
      }
      case kTessEvalShader: {
        uri::Uri uri;
        if(!uri::TryParseUri(uri, value, "shader")) {
          DLOG(ERROR) << "failed to parse fragment shader uri: " << value;
          return TransitionTo(kError);
        }
        return OnParseProgramShaderRef(shader::kTessEvalShader, uri);
      }
      case kTessControlShader: {
        uri::Uri uri;
        if(!uri::TryParseUri(uri, value, "shader")) {
          DLOG(ERROR) << "failed to parse fragment shader uri: " << value;
          return TransitionTo(kError);
        }
        return OnParseProgramShaderRef(shader::kTessControlShader, uri);
      }
      default:
        return json::ReaderHandlerTemplate<ProgramReaderState, ProgramReaderHandler>::String(value, length, b);
    }
  }

  bool ProgramReaderHandler::OnParseDataField(const std::string& name) {
    if(EqualsIgnoreCase(name, "fragment")) {
      return TransitionTo(kFragmentShader);
    } else if(EqualsIgnoreCase(name, "vertex")) {
      return TransitionTo(kVertexShader);
    } else if(EqualsIgnoreCase(name, "geometry")) {
      return TransitionTo(kGeometryShader);
    } else if(EqualsIgnoreCase(name, "tesseval")) {
      return TransitionTo(kTessEvalShader);
    } else if(EqualsIgnoreCase(name, "tesscontrol")) {
      return TransitionTo(kTessControlShader);
    } else if(EqualsIgnoreCase(name, "include")) {
      return TransitionTo(kIncludedShaders);
    }
    
    DLOG(ERROR) << "unexpected field: " << name;
    return TransitionTo(kError);
  }

  bool ProgramReaderHandler::OnParseProgramShaderRef(const shader::ShaderType type, const uri::Uri& uri) {
    shaders_.emplace_back(type, uri);
    return TransitionTo(kData);
  }

  bool ProgramReaderHandler::OnParseType(const std::string& name) {
    if(!EqualsIgnoreCase(name, "Program"))
      return TransitionTo(kError);
    return TransitionTo(kOpen);
  }

  bool ProgramReaderHandler::OnParseMetaName(const std::string& name) {
    name_ = name;
    return TransitionTo(kMeta);
  }

  bool ProgramReaderHandler::OnParseMetaTag(const std::string& value) {
    tags_.push_back(value);
    return TransitionTo(kMetaTags);
  }
}