#include "mcc/uri_parser.h"

namespace mcc::uri {
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
      case '&':
      case '#':
      case '\0':
      case EOF:
        if(!OnParseQuery0(num_query_params_++, key))
          return false;
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
    if(!OnParseQuery1(num_query_params_++, key, value))
      return false;
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

  ParseResult Parser::Parse() {
    if(!ParseScheme()) {
      if(!config_.default_scheme)
        return ParseResult::Failure("Failed to parse uri scheme, no default scheme.");
      if(!OnParseScheme(config_.default_scheme, strlen(config_.default_scheme)))
        return ParseResult::Failure(fmt::format("Failed to parse uri scheme: {0:s}", config_.default_scheme));
    } else {
      if(!OnParseScheme((const char*) token_, token_len_))
        return ParseResult::Failure(fmt::format("Failed to parse uri scheme: {0:s}", std::string((const char*) token_, token_len_)));
    }

    if(!ParsePath())
      return ParseResult::Failure("Failed to parse uri path.");
    if(!OnParsePath((const char*) token_, token_len_))
      return ParseResult::Failure(fmt::format("Failed to parse uri path: {0:s}", std::string((const char*) token_, token_len_)));

    do {
      switch(PeekChar()) {
        case '?': {
          if(!config_.parse_queries)
            return ParseResult::Failure("unexpected token '?', uri queries are not enabled.");
          NextChar();
          if(!ParseQueryParameterList())
            return ParseResult::Failure("failed to parse uri query parameter list.");
          continue;
        }
        case '#': {
          if(!config_.parse_fragments)
            return ParseResult::Failure("unexpected token '#', uri fragments are not enabled.");

          NextChar();
          if(!ParseFragment())
            return ParseResult::Failure("failed to parse uri fragment.");
          if(!OnParseFragment((const char*) token_, token_len_))
            return ParseResult::Failure(fmt::format("failed to parse uri fragment: {0:s}", std::string((const char*) token_, token_len_)));
          continue;
        }
        case EOF:
        case '\0':
          return ParseResult::Success();
        default:
          return ParseResult::Failure(fmt::format("unexpected token: '{0:c}'", NextChar()));
      }
    } while(true);
    return ParseResult::Failure("unexpected end of stream.");
  }
}