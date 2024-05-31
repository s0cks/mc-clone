#ifndef MCC_SHADER_JSON_H
#define MCC_SHADER_JSON_H

#include <cstdint>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>

namespace mcc::shader {
#define FOR_EACH_SHADER_READER_STATE(V)       \
  V(Open)                                     \
  V(ExpectValue)                              \
  V(Closed)

  class ShaderReaderHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, ShaderReaderHandler> {
  public:
    enum State : uint8_t {
#define DEFINE_STATE(Name) k##Name,
      FOR_EACH_SHADER_READER_STATE(DEFINE_STATE)
#undef DEFINE_STATE
    };
  protected:
    State state_;

    inline State GetState() const {
      return state_;
    }

    inline void SetState(const State state) {
      state_ = state;
    }

#define DEFINE_STATE_CHECK(Name)                                             \
    inline bool Is##Name() const { return GetState() == State::k##Name; }
    FOR_EACH_SHADER_READER_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK
  public:
    ShaderReaderHandler():
      state_(kClosed) {
    }
    virtual ~ShaderReaderHandler() = default;

    bool StartObject() {
      if(!IsClosed())
        return false;
      SetState(kOpen);
      return true;
    }

    bool EndObject(const rapidjson::SizeType size) {
      if(!IsOpen())
        return false;
      SetState(State::kClosed);
      return true;
    }

    bool Default() {
      return false;
    }
  };
}

#endif //MCC_SHADER_JSON_H