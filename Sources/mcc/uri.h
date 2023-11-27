#ifndef MCC_URI_H
#define MCC_URI_H

#include <string>
#include "mcc/buffer.h"
#include "mcc/murmur.h"

namespace mcc {
  namespace uri {
    typedef std::string basic_uri;
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

    class UriParser {
    public:
      typedef std::function<bool(const UriParser* parser, const std::string& value)> SchemeParsedCallback;
      typedef std::function<bool(const UriParser* parser, const std::string& value)> PathParsedCallback;
      typedef std::function<bool(const UriParser* parser, const uint64_t idx, const std::string& name, const std::string& value)> QueryParsedCallback;
      typedef std::function<bool(const UriParser* parser, const std::string& value)> FragmentParsedCallback;

      struct Config {
        SchemeParsedCallback on_scheme_parsed;
        PathParsedCallback on_path_parsed;
        QueryParsedCallback on_query_parsed;
        FragmentParsedCallback on_fragment_parsed;
      };
      //TODO: authority?
    protected:
      Config config_;
      void* data_;

      explicit UriParser(const Config& config):
        config_(config) {
      }
    public:
      virtual ~UriParser() = default;
      virtual bool Parse() = 0;
    };
  }
}

#endif //MCC_URI_H