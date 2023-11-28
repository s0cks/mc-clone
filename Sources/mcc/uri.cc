#include <string>
#include <unordered_set>

#include "mcc/uri.h"

namespace mcc::uri {
  static inline bool
  OnSchemeParsed(const Parser* parser, const std::string& scheme) {
    DLOG(INFO) << "parsed scheme: " << scheme;
    auto uri = (Uri*)parser->data();
    uri->scheme = scheme;
    return true;
  }

  static inline bool
  OnPathParsed(const Parser* parser, const std::string& path) {
    DLOG(INFO) << "parsed path: " << path;
    auto uri = (Uri*)parser->data();
    uri->path = path;
    return true;
  }

  static inline bool
  OnQueryParsed0(const Parser* parser, const uint64_t idx, const std::string& key) {
    auto uri = (Uri*)parser->data();
    const auto pos = uri->query.insert({ key, "" });
    if(!pos.second) {
      DLOG(ERROR) << "failed to insert query parameter #" << idx << " " << key;
      return false;
    }
    return true;
  }

  static inline bool
  OnQueryParsed1(const Parser* parser, const uint64_t idx, const std::string& key, const std::string& value) {
    auto uri = (Uri*)parser->data();
    const auto pos = uri->query.insert({ key, value });
    if(!pos.second) {
      DLOG(ERROR) << "failed to insert query parameter #" << idx << " " << key << "=" << value;
      return false;
    }
    return true;
  }

  static inline bool
  OnFragmentParsed(const Parser* parser, const std::string& value) {
    DLOG(INFO) << "parsed fragment: " << value;
    auto uri = (Uri*)parser->data();
    uri->fragment = value;
    return true;
  }

  Uri::Uri(const basic_uri& uri):
    scheme(),
    path() {
    Parser::Config config = {
      .OnSchemeParsed = &OnSchemeParsed,
      .OnPathParsed = &OnPathParsed,
      .OnQueryParsed0 = &OnQueryParsed0,
      .OnQueryParsed1 = &OnQueryParsed1,
      .OnFragmentParsed = &OnFragmentParsed,
    };
    Parser parser(config, uri, this);
    DLOG_IF(ERROR, !parser.Parse()) << "failed to parse uri: " << uri;
  }

  bool Parser::ParseScheme() {
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

  bool Parser::ParsePath() {
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

  bool Parser::ParseQueryParameterKey() {
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

  bool Parser::ParseQueryParameterValue() {
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

  bool Parser::ParseQueryParameter() {
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

  bool Parser::ParseQueryParameterList() {
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

  bool Parser::ParseFragment() {
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

  bool Parser::Parse() {
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