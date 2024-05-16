#ifndef MCC_PROPERTIES_PARSER_H
#define MCC_PROPERTIES_PARSER_H

#include "mcc/parser.h"
#include "mcc/properties/property_set.h"

namespace mcc::properties {
  static constexpr const auto kDefaultParserBufferSize = 4096;
  static constexpr const auto kDefaultTokenBufferSize = 1024;
  class Parser : public FileParserTemplate<kDefaultParserBufferSize, kDefaultTokenBufferSize> {
  public:
    explicit Parser(FILE* file, void* data = nullptr):
      FileParserTemplate<kDefaultParserBufferSize, kDefaultTokenBufferSize>(file, data) {  
    }
    ~Parser() override = default;

    bool ParseProperties(PropertySet& properties);
  };
}

#endif //MCC_PROPERTIES_PARSER_H