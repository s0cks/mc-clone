#include "mcc/program/program_json.h"
#include <glog/logging.h>

namespace mcc::program {
  bool ProgramReaderHandler::String(const char* value, const rapidjson::SizeType length, const bool) {
    switch(GetState()) {
      case kOpen:
        return OnParseField(std::string(value, length));
      case kExpectVertex:
        return OnParseVertexShader(std::string(value, length));
      case kExpectFragment:
        return OnParseFragmentShader(std::string(value, length));
      default:
        return false;
    }
  }

  bool ProgramReaderHandler::OnParseField(const std::string& name) {
    DLOG(INFO) << "parsed field: " << name;
    if(EqualsIgnoreCase(name, "fragment")) {
      SetState(kExpectFragment);
      return true;
    } else if(EqualsIgnoreCase(name, "vertex")) {
      SetState(kExpectVertex);
      return true;
    }
    LOG(ERROR) << "unexpected field: " << name;
    return false;
  }

  bool ProgramReaderHandler::OnParseVertexShader(const std::string& value) {
    DLOG(INFO) << "parsed vertex shader: " << value;

    const auto uri = uri::Uri(value); //TODO: need to default scheme to shader, and extension to .vert
    shaders_.emplace_back(shader::kVertexShader, uri);

    SetState(kOpen);
    return true;
  }

  bool ProgramReaderHandler::OnParseFragmentShader(const std::string& value) {
    DLOG(INFO) << "parsed fragment shader: " << value;

    const auto uri = uri::Uri(value); //TODO: need to default scheme to shader, and extension to .frag
    shaders_.emplace_back(shader::kFragmentShader, uri);

    SetState(kOpen);
    return true;
  }
}