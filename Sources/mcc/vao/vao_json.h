#ifndef MCC_VAO_JSON_H
#define MCC_VAO_JSON_H

#include <cstdint>
#include <ostream>
#include "mcc/json.h"
#include "mcc/metadata.h"

namespace mcc::vao {
#define FOR_EACH_VAO_READER_STATE(V)        \
  V(Error)                                  \
  V(Open)                                   \
  V(Type)                                   \
  V(Meta)                                   \
  V(MetaName)                               \
  V(MetaTags)                               \
  V(Data)                                   \
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
    Metadata meta_;

#define DEFINE_STATE_CHECK(Name)                                            \
    inline bool Is##Name() const { return GetState() == VaoReaderState::k##Name; }
    FOR_EACH_VAO_READER_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK

    bool OnParseDataField(const std::string& name) override;
    bool OnParseType(const std::string& type) override;
  public:
    VaoReaderHandler() = default;
    virtual ~VaoReaderHandler() = default;

    const Metadata& GetMeta() const {
      return meta_;
    }
  };
}

#endif //MCC_VAO_JSON_H