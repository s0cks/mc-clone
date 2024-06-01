#ifndef MCC_VAO_JSON_H
#define MCC_VAO_JSON_H

#include <cstdint>
#include <ostream>
#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>

namespace mcc::vao {
#define FOR_EACH_VAO_READER_STATE(V)        \
  V(Error)                                  \
  V(Open)                                   \
  V(Closed)

  class VaoReaderHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, VaoReaderHandler> {
  public:
    enum State : uint16_t {
#define DEFINE_STATE(Name) k##Name,
      FOR_EACH_VAO_READER_STATE(DEFINE_STATE)
#undef DEFINE_STATE
    };

    inline friend std::ostream&
    operator<<(std::ostream& stream, const State& rhs) {
      switch(rhs) {
#define DEFINE_TO_STRING(Name) \
        case k##Name: return stream << #Name;
        FOR_EACH_VAO_READER_STATE(DEFINE_TO_STRING)
#undef DEFINE_TO_STRING
        default: return stream << "Unknown VaoReaderHandler::State: " << static_cast<uint16_t>(rhs);
      }
    }
  protected:
    State state_;

    inline void SetState(const State& rhs) {
      state_ = rhs;
    }

    inline State GetState() const {
      return state_;
    }

#define DEFINE_STATE_CHECK(Name)                                            \
    inline bool Is##Name() const { return GetState() == State::k##Name; }
    FOR_EACH_VAO_READER_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK

    bool TransitionTo(const State state);
    bool OnParseField(const std::string& name);
  public:
    VaoReaderHandler():
      state_(kClosed) {
    }
    virtual ~VaoReaderHandler() = default;
    bool StartObject();
    bool EndObject(const rapidjson::SizeType size);
    bool String(const char* value, const rapidjson::SizeType length, const bool);
    bool Default();
  };
}

#endif //MCC_VAO_JSON_H