#ifndef URI_PARSER_H
#define URI_PARSER_H

#include <cstdint>
#include <ostream>

#include "mcc/uri.h"
#include "mcc/parser.h"

namespace mcc::uri {
#define FOR_EACH_URI_PARSER_ERROR(V)  \
  V(UnexpectedToken)                  \
  V(UnexpectedEndOfStream)

  class ParseResult {
  protected:
    bool success_;
    std::string message_;
  public:
    ParseResult() = default;
    ParseResult(const bool success, const std::string& message):
      success_(success),
      message_(message) {
    }
    ParseResult(const ParseResult& rhs) = default;
    ~ParseResult() = default;

    bool IsSuccess() const {
      return success_;
    }

    const std::string& GetMessage() const {
      return message_;
    }

    operator bool() const {
      return IsSuccess();
    }

    ParseResult& operator=(const ParseResult& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const ParseResult& rhs) {
      stream << "uri::ParseResult(";
      stream << "success=" << (rhs.IsSuccess() ? 'y' : 'n');
      if(!rhs)
        stream << ", message=" << rhs.GetMessage();
      stream << ")";
      return stream;
    }
  public:
    static inline ParseResult
    Success(const std::string& message = "Success.") {
      return ParseResult(true, message);
    }

    static inline ParseResult
    Failure(const std::string& message) {
      return ParseResult(false, message);
    }
  };

  static constexpr const uint64_t kDefaultParserBufferSize = 4096;
  static constexpr const uint64_t kDefaultTokenBufferSize = 1024;
  class Parser : public ParserTemplate<kDefaultParserBufferSize, kDefaultTokenBufferSize> {
  public:
    struct Config {
      const char* default_scheme;
      bool parse_queries;
      bool parse_fragments;
      bool (*OnParseScheme)(const Parser* parser, const char* scheme, const uint64_t length);
      bool (*OnParsePath)(const Parser* parser, const char* path, const uint64_t length);
      bool (*OnParseQuery0)(const Parser* parser, const uint64_t idx, const std::string& key);
      bool (*OnParseQuery1)(const Parser* parser, const uint64_t idx, const std::string& key, const std::string& value);
      bool (*OnParseFragment)(const Parser* parser, const char* fragment, const uint64_t length);
      bool (*OnParseError)(const Parser* parser);
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

    template<typename Func, typename... Args>
    inline bool
    CallIfExists(Func* func, Args... args) const {
      return func ? func(this, args...) : true;
    }

    inline bool
    OnParseScheme(const char* scheme, const uint64_t length) const {
      return CallIfExists(config_.OnParseScheme, scheme, length);
    }

    inline bool
    OnParsePath(const char* path, const uint64_t length) const {
      return CallIfExists(config_.OnParsePath, path, length);
    }

    inline bool
    OnParseQuery0(const uint64_t idx, const std::string& key) const {
      return CallIfExists(config_.OnParseQuery0, idx, key);
    }

    inline bool
    OnParseQuery1(const uint64_t idx, const std::string& key, const std::string& value) const {
      return CallIfExists(config_.OnParseQuery1, idx, key, value);
    }

    inline bool
    OnParseFragment(const char* fragment, const uint64_t length) const {
      return CallIfExists(config_.OnParseFragment, fragment, length);
    }

    inline bool
    OnParseError() const {
      return CallIfExists(config_.OnParseError);
    }
  public:
    explicit Parser(const Config& config, const basic_uri& uri, void* data = nullptr):
      ParserTemplate(data, uri),
      config_(config),
      num_query_params_(0) {
    }
    Parser(const basic_uri& uri, void* data = nullptr):
      ParserTemplate(data, uri),
      config_(),
      num_query_params_(0) {
    }
    ~Parser() override = default;
    ParseResult Parse();
  };
}

#endif //URI_PARSER_H