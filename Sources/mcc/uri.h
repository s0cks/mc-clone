#ifndef MCC_URI_H
#define MCC_URI_H

#include <string>
#include "mcc/buffer.h"
#include "mcc/murmur.h"
#include "mcc/parser.h"

namespace mcc {
  namespace uri {
    typedef std::string basic_uri;
    
    class UriParser;
    struct Uri {
      std::string protocol;
      std::string location;

      Uri() = default;
      Uri(const basic_uri& uri);
      Uri(const Uri& rhs) = default;
      ~Uri() = default;

      Uri& operator=(const Uri& rhs) = default;

      bool operator==(const Uri& rhs) const {
        return protocol == rhs.protocol
            && location == rhs.location;
      }

      bool operator!=(const Uri& rhs) const {
        return protocol != rhs.protocol
            || location != rhs.location;
      }

      friend std::ostream& operator<<(std::ostream& stream, const Uri& rhs) {
        stream << "Uri(";
        stream << "protocol=" << rhs.protocol << "://" << rhs.location;
        stream << ")";
        return stream;
      }
    };

    class Protocol {
    protected:
      Protocol() = default;
    public:
      virtual ~Protocol() = default;
      virtual std::string GetName() const = 0;
      virtual bool Matches(const basic_uri& protocol) const = 0;
      virtual std::string Resolve(const Uri& uri) const = 0;
    };

    typedef std::shared_ptr<Protocol> ProtocolPtr;

    bool RegisterProtocol(const ProtocolPtr& protocol);
  
    template<class P>
    static inline bool
    RegisterProtocol() {
      return RegisterProtocol(std::make_shared<P>());
    }

    static constexpr const uint64_t kDefaultParserBufferSize = 4096;
    static constexpr const uint64_t kDefaultTokenBufferSize = 1024;
    class UriParser : public ParserTemplate<kDefaultParserBufferSize, kDefaultTokenBufferSize> {
    public:
      struct Config {
        bool (*OnSchemeParsed)(const UriParser* parser, const std::string& value);
        bool (*OnPathParsed)(const UriParser* parser, const std::string& value);
        bool (*OnQueryParsed0)(const UriParser* parser, const uint64_t idx, const std::string& key);
        bool (*OnQueryParsed1)(const UriParser* parser, const uint64_t idx, const std::string& key, const std::string& value);
        bool (*OnFragmentParsed)(const UriParser* parser, const std::string& value);
      };
    protected:
      Config config_;
      uint64_t num_query_params_;

      bool ParseScheme();
      bool ParsePath();
      bool ParseQueryParameterKey();
      bool ParseQueryParameterValue();
      bool ParseQueryParameter();
      bool ParseQueryParameterList();
      bool ParseFragment();

      static inline bool
      IsQueryDelimiter(const char c) {
        switch(c) {
          case '&':
          case ';':
            return true;
          default:
            return false;
        }
      }
    public:
      explicit UriParser(const Config& config, const std::string& buffer, void* data = nullptr):
        ParserTemplate(data, buffer),
        config_(config),
        num_query_params_(0) {
      }
      UriParser(const std::string& buffer, void* data = nullptr):
        ParserTemplate(data, buffer),
        config_(),
        num_query_params_(0) {
      }
      ~UriParser() override = default;
      bool Parse();
    };
  }
}

#endif //MCC_URI_H