#include "mcc/program/program_json.h"
#include <glog/logging.h>

namespace mcc::program {
  bool ProgramReaderHandler::TransitionTo(const State state) {
    SetState(state);
    return state != kError;
  }

  bool ProgramReaderHandler::Default() {
    return TransitionTo(kError);
  }

  bool ProgramReaderHandler::StartObject() {
    if(!IsClosed()) {
      DLOG(ERROR) << "program object is not closed.";
      return TransitionTo(kError);
    }
    return TransitionTo(kOpen);
  }

  bool ProgramReaderHandler::EndObject(const rapidjson::SizeType size) {
    if(!IsOpen()) {
      DLOG(ERROR) << "program object is not open.";
      return TransitionTo(kError);
    }
    return TransitionTo(kClosed);
  }

  bool ProgramReaderHandler::StartArray() {
    switch(GetState()) {
      case kIncludedShaders:
        return true;
      default:
        DLOG(ERROR) << "cannot start array at state: " << GetState();
        return TransitionTo(kError);
    }
  }

  bool ProgramReaderHandler::EndArray(const rapidjson::SizeType size) {
    switch(GetState()) {
      case kIncludedShaders:
        return TransitionTo(kOpen);
      default:
        DLOG(ERROR) << "cannot close array at state: " << GetState();
        return TransitionTo(kError);
    }
  }

  bool ProgramReaderHandler::String(const char* value, const rapidjson::SizeType length, const bool) {
    switch(GetState()) {
      case kOpen:
        return OnParseField(std::string(value, length));
      case kIncludedShaders: {
        uri::Uri uri;
        if(!uri::TryParseUri(uri, value, "shader")) {
          DLOG(ERROR) << "failed to parse fragment shader uri: " << value;
          return TransitionTo(kError);
        }
        DLOG(INFO) << "parsed included shader ref: " << uri;
        return true;
      }
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
        return TransitionTo(kError);
    }
  }

  bool ProgramReaderHandler::OnParseField(const std::string& name) {
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
    return TransitionTo(kOpen);
  }
}