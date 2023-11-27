#include <string>
#include <unordered_set>

#include "mcc/uri.h"

namespace mcc::uri {
  struct ProtocolHasher {
    std::size_t operator()(const ProtocolPtr& k) const {
      using std::size_t;
      using std::hash;
      using std::string;
      return hash<string>()(k->GetName());
    }
  };

  struct ProtocolEquals {
    bool operator()(const ProtocolPtr& lhs, const ProtocolPtr& rhs) const {
      if(!lhs && !rhs)
        return true;
      if((lhs && !rhs) || (!lhs && rhs))
        return false;
      return lhs->GetName() == rhs->GetName();
    }
  };

  static std::unordered_set<ProtocolPtr, ProtocolHasher, ProtocolEquals> protocols_;

  static inline std::optional<ProtocolPtr>
  ParseUriProtocol(const basic_uri& data) {
    const auto first_colon = data.find_first_of(':');
    if(first_colon == std::string::npos)
      return std::nullopt;
    const auto protoc = data.substr(0, first_colon);
    for(const auto& protocol : protocols_) {
      if(protocol->Matches(protoc))
        return protocol;
    }
    DLOG(ERROR) << "cannot find protocol '" << protoc << "'";
    return std::nullopt;
  }

  static inline void
  ParseProtocol(const basic_uri& uri, std::string* result) {
    for(const auto& protocol : protocols_) {
      DLOG(INFO) << "checking if " << uri << " matches: " << protocol->GetName();
      if(protocol->Matches(uri)) {
        (*result) = protocol->GetName();
        return;
      }
    }
    DLOG(ERROR) << "cannot find protocol for " << uri;
    (*result) = "file";
  }

  static inline void
  ParseLocation(const basic_uri& uri, std::string* result) {
    auto first_colon = uri.find_first_of(':');
    if(first_colon == std::string::npos) {
      (*result) = uri;
      return;
    }
    if(uri[first_colon + 1] == '/')
      first_colon += 1;
    if(uri[first_colon + 1] == '/')
      first_colon += 1;
    (*result) = uri.substr(first_colon + 1);
  }

  static inline bool
  OnSchemeParsed(const UriParser* parser, const std::string& scheme) {
    DLOG(INFO) << "parsed scheme: " << scheme;
    auto uri = (Uri*)parser->data();
    uri->protocol = scheme;
    return true;
  }

  static inline bool
  OnPathParsed(const UriParser* parser, const std::string& path) {
    DLOG(INFO) << "parsed path: " << path;
    auto uri = (Uri*)parser->data();
    uri->location = path;
    return true;
  }

  Uri::Uri(const basic_uri& uri):
    protocol(),
    location() {
    UriParser::Config config = {
      .OnSchemeParsed = &OnSchemeParsed,
      .OnPathParsed = &OnPathParsed,
    };
    UriParser parser(config, uri, this);
    DLOG_IF(ERROR, !parser.Parse()) << "failed to parse uri: " << uri;
  }

  bool RegisterProtocol(const ProtocolPtr& protocol) {
    if(!protocol) {
      DLOG(ERROR) << "cannot register a null protocol.";
      return false;
    }
    const auto pos = protocols_.insert(protocol);
    DLOG_IF(WARNING, !pos.second) << "failed to register the '" << protocol->GetName() << "' protocol";
    return pos.second;
  }

  bool UriParser::ParseScheme() {
    token_len_ = 0;
    do {
      auto next = NextChar();
      switch(next) {
        case ':': {
          switch((next = PeekChar())) {
            case '/':
              NextChar();
              switch((next = PeekChar())) {
                case '/':
                  NextChar();
                  break;
                default:
                  DLOG(ERROR) << "unexpected token: " << next;
                  return false;
              }
            default:
              break;
          }
          token_[token_len_] = '\0';
          return token_len_ > 0;
        }
        case EOF:
          DLOG(ERROR) << "unexpected EOF";
          return false;
        default: {
          token_[token_len_++] = next;
          continue;
        }
      }
    } while(true);
  }

  bool UriParser::ParsePath() {
    token_len_ = 0;
    do {
      switch(PeekChar()) {
        case '?':
        case EOF:
          token_[token_len_] = '\0';
          return true;
        default:
          token_[token_len_++] = NextChar();
          continue;
      }
    } while(true);
    return false;
  }

  bool UriParser::ParseQueryParameterKey() {
    token_len_ = 0;
    do {
      switch(PeekChar()) {
        case '#':
        case '=':
        case '\0':
        case EOF:
          return true;
        default:
          token_[token_len_++] = NextChar();
          continue;
      }
    } while(true);
    return false;
  }

  bool UriParser::ParseQueryParameterValue() {
    token_len_ = 0;
    do {
      switch(PeekChar()) {
        case '&':
          NextChar();
        case '#':
        case '\0':
        case EOF:
          return true;
        default:
          token_[token_len_++] = NextChar();
          continue;
      }
    } while(true);
    return false;
  }

  bool UriParser::ParseQueryParameter() {
    if(!ParseQueryParameterKey()) {
      DLOG(ERROR) << "failed to parse query parameter key.";
      return false;
    }
    const auto key = std::string((const char*) token_, token_len_);
    
    switch(PeekChar()) {
      case '=':
        NextChar();
        break;
      case '#':
      case '\0':
      case EOF:
        if(config_.OnQueryParsed0) {
          if(!config_.OnQueryParsed0(this, num_query_params_++, key))
            return false;
        }
        return true;
      default:
        DLOG(ERROR) << "unexpected token: " << NextChar();
        return false;
    }

    if(!ParseQueryParameterValue()) {
      DLOG(ERROR) << "failed to parse query parameter value.";
      return false;
    }
    const auto value = std::string((const char*) token_, token_len_);
    if(config_.OnQueryParsed1) {
      if(!config_.OnQueryParsed1(this, num_query_params_++, key, value))
        return false;
    }
    return true;
  }

  bool UriParser::ParseQueryParameterList() {
    do {
      switch(PeekChar()) {
        case '#':
        case '\0':
        case EOF:
          return true;
        default:
          if(!ParseQueryParameter()) {
            DLOG(ERROR) << "failed to parse query: " << std::string((const char*) &buffer_[rpos_], buffer_len_ - rpos_);
            return false;
          }
          continue;
      }
    } while(true);
    return false;
  }

  bool UriParser::ParseFragment() {
    token_len_ = 0;
    do {
      switch(PeekChar()) {
        case '\0':
        case EOF:
          return true;
        default:
          token_[token_len_++] = NextChar();
          continue;
      }
    } while(true);
    return false;
  }

  bool UriParser::Parse() {
    if(!ParseScheme()) {
      DLOG(ERROR) << "failed to parse scheme.";
      return false;
    }
    const auto scheme = std::string((const char*) token_, token_len_);
    if(config_.OnSchemeParsed)
      if(!config_.OnSchemeParsed(this, scheme))
        return false;

    if(!ParsePath()) {
      DLOG(ERROR) << "failed to parse location.";
      return false;
    }
    const auto location = std::string((const char*) token_, token_len_);
    if(config_.OnPathParsed) {
      if(!config_.OnPathParsed(this, location))
        return false;
    }

    do {
      switch(PeekChar()) {
        case '?': {
          NextChar();
          if(!ParseQueryParameterList()) {
            DLOG(ERROR) << "failed to parse query.";
            return false;
          }
          continue;
        }
        case '#': {
          NextChar();
          if(!ParseFragment()) {
            DLOG(ERROR) << "failed to parse fragment.";
            return false;
          }
          const auto fragment = std::string((const char*) token_, token_len_);
          if(config_.OnFragmentParsed) {
            if(!config_.OnFragmentParsed(this, fragment))
              return false;
          }
          continue;
        }
        case EOF:
        case '\0':
          return true;
        default:
          DLOG(ERROR) << "unexpected token: " << NextChar();
          return false;
      }
    } while(true);
    return false;
  }
}