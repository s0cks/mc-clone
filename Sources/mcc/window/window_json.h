#ifndef MCC_WINDOW_JSON_H
#define MCC_WINDOW_JSON_H

#include "mcc/json.h"

namespace mcc::window {
#define FOR_EACH_WINDOW_READER_STATE(V)   \
  FOR_EACH_JSON_READER_TEMPLATE_STATE(V)  \
  V(Size)                                 \
  V(Title)                                \
  V(Resizable)                            \
  V(Fullscreen)

  enum class WindowReaderState {
#define DEFINE_STATE(Name) k##Name,
    FOR_EACH_WINDOW_READER_STATE(DEFINE_STATE)
#undef DEFINE_STATE
  };

  static inline std::ostream&
  operator<<(std::ostream& stream, const WindowReaderState& rhs) {
    switch(rhs) {
#define DEFINE_TO_STRING(Name) \
      case WindowReaderState::k##Name: return stream << #Name;
      FOR_EACH_WINDOW_READER_STATE(DEFINE_TO_STRING)
#undef DEFINE_TO_STRING
      default: return stream << "Unknown WindowReaderState: " << static_cast<uword>(rhs);
    }
  }

  class WindowReaderHandler : public json::ReaderHandlerTemplate<WindowReaderState, WindowReaderHandler> {
  protected:
#define DEFINE_STATE_CHECK(Name)  \
    inline bool Is##Name() const { return GetState() == WindowReaderState::k##Name; }
    FOR_EACH_WINDOW_READER_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK

#define DEFINE_TRANSITION_TO(Name)  \
    inline bool TransitionTo##Name() { return TransitionTo(WindowReaderState::k##Name); }
    FOR_EACH_WINDOW_READER_STATE(DEFINE_TRANSITION_TO)
#undef DEFINE_TRANSITION_TO

    bool OnParseDataField(const std::string& name) override;
    bool OnParseResizable(const bool value);
    bool OnParseFullscreen(const bool value);
    bool OnParseTitle(const std::string& value);
    bool OnParseSize(const std::string& value);
  public:
    ~WindowReaderHandler() override = default;
    bool Bool(const bool value) override;
    bool String(const char* value, const rapidjson::SizeType length, const bool) override;
  };
}

#endif //MCC_WINDOW_JSON_H