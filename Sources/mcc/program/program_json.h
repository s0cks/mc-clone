#ifndef MCC_PROGRAM_JSON_H
#define MCC_PROGRAM_JSON_H

#include <vector>
#include <ostream>
#include <glog/logging.h>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>

#include "mcc/common.h"
#include "mcc/shader/shader_type.h"

namespace mcc::program {
#define FOR_EACH_PROGRAM_READER_STATE(V)        \
  V(Closed)                                     \
  V(Open)                                       \
  V(ExpectFragment)                             \
  V(ExpectVertex)

  class ProgramShader {
    friend class ProgramReaderHandler;
    friend class std::allocator<ProgramShader>;
    DEFINE_DEFAULT_COPYABLE_TYPE(ProgramShader);
  protected:
    shader::ShaderType type_;
    uri::Uri uri_;

    ProgramShader(const shader::ShaderType type,
                  const uri::Uri& uri):
      type_(type),
      uri_(uri) {
    }
  public:
    ProgramShader() = default;
    ~ProgramShader() = default;

    shader::ShaderType GetType() const {
      return type_;
    }

    const uri::Uri& GetUri() const {
      return uri_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const ProgramShader& rhs) {
      stream << "ProgramShader(";
      stream << "type=" << rhs.GetType() << ", ";
      stream << "uri=" << rhs.GetUri();
      stream << ")";
      return stream;
    }
  };
  typedef std::vector<ProgramShader> ProgramShaderList;

  class ShaderCode;
  class ProgramReaderHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, ProgramReaderHandler> {
  public:
    enum State {
#define DEFINE_STATE(Name) k##Name,
      FOR_EACH_PROGRAM_READER_STATE(DEFINE_STATE)
#undef DEFINE_STATE
    };
    
    inline friend std::ostream&
    operator<<(std::ostream& stream, const State& rhs) {
      switch(rhs) {
#define DEFINE_TO_STRING(Name) \
        case k##Name: return stream << #Name;
        FOR_EACH_PROGRAM_READER_STATE(DEFINE_TO_STRING)
#undef DEFINE_TO_STRING
        default: return stream << "Unknown ProgramReaderHandler::State";
      }
    }
  protected:
    State state_;
    ProgramShaderList shaders_;

    inline void SetState(const State& rhs) {
      state_ = rhs;
    }

    inline State GetState() const {
      return state_;
    }

#define DEFINE_STATE_CHECK(Name)                                            \
    inline bool Is##Name() const { return GetState() == State::k##Name; }
    FOR_EACH_PROGRAM_READER_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK

    bool OnParseField(const std::string& name);

    bool OnParseVertexShader(const std::string& value);
    bool OnParseFragmentShader(const std::string& value);
  public:
    ProgramReaderHandler():
      state_(kClosed),
      shaders_() {
    }
    ~ProgramReaderHandler() = default;

    bool StartObject() {
      DLOG(INFO) << __FUNCTION__;
      DLOG(INFO) << "state: " << GetState();
      if(!IsClosed()) {
        DLOG(ERROR) << "program object is not closed.";
        return false;
      }

      SetState(kOpen);
      return true;
    }

    bool EndObject(const rapidjson::SizeType size) {
      DLOG(INFO) << __FUNCTION__;
      DLOG(INFO) << "state: " << GetState();
      if(!IsOpen()) {
        DLOG(ERROR) << "program object is not open.";
        return false;
      }

      SetState(kClosed);
      return true;
    }

    bool String(const char* value, const rapidjson::SizeType length, const bool);

    bool Default() {
      DLOG(INFO) << __FUNCTION__;
      DLOG(INFO) << "state: " << GetState();
      return false;
    }

    const ProgramShaderList& shaders() const {
      return shaders_;
    }

    ProgramShaderList::const_iterator shaders_begin() const {
      return shaders_.begin();
    }

    ProgramShaderList::const_iterator shaders_end() const {
      return shaders_.end();
    }
  };
}

#endif //MCC_PROGRAM_JSON_H