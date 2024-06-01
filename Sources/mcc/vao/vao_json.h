#ifndef MCC_VAO_JSON_H
#define MCC_VAO_JSON_H

#include <cstdint>
#include <ostream>
#include "mcc/json.h"

namespace mcc::vao {
#define FOR_EACH_VAO_READER_STATE(V)        \
  V(Error)                                  \
  V(Open)                                   \
  V(SpecType)                               \
  V(SpecMeta)                               \
  V(SpecMetaName)                           \
  V(SpecMetaTags)                           \
  V(SpecData)                               \
  V(Closed)

  enum VaoReaderState : uint16_t {
#define DEFINE_STATE(Name) k##Name,
    FOR_EACH_VAO_READER_STATE(DEFINE_STATE)
#undef DEFINE_STATE
  };

  static inline std::ostream&
  operator<<(std::ostream& stream, const VaoReaderState& rhs) {
    switch(rhs) {
#define DEFINE_TO_STRING(Name) \
      case k##Name: return stream << #Name;
      FOR_EACH_VAO_READER_STATE(DEFINE_TO_STRING)
#undef DEFINE_TO_STRING
      default: return stream << "Unknown VaoReaderHandler::State: " << static_cast<uint16_t>(rhs);
    }
  }

  class VaoReaderHandler : public json::ReaderHandlerTemplate<VaoReaderState, VaoReaderHandler> {
  protected:
#define DEFINE_STATE_CHECK(Name)                                            \
    inline bool Is##Name() const { return GetState() == VaoReaderState::k##Name; }
    FOR_EACH_VAO_READER_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK

    bool OnParseField(const std::string& name);
    bool OnParseMetaName(const std::string& name);
    bool OnParseMetaTag(const std::string& value);
  public:
    VaoReaderHandler() = default;
    virtual ~VaoReaderHandler() = default;
    bool StartArray() override;
    bool EndArray(const rapidjson::SizeType size) override;
    bool StartObject() override;
    bool EndObject(const rapidjson::SizeType size) override;
    bool String(const char* value, const rapidjson::SizeType length, const bool) override;
    bool Default() override;
  };
}

#endif //MCC_VAO_JSON_H