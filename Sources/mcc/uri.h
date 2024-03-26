#ifndef MCC_URI_H
#define MCC_URI_H

#include <cstdio>
#include <string>
#include <optional>

#include "mcc/buffer.h"
#include "mcc/murmur.h"
#include "mcc/parser.h"

namespace mcc {
  namespace uri {
    typedef std::string basic_uri;
    
    class Parser;
    struct Uri {
      std::string scheme;
      std::string path;
      std::string fragment;
      std::unordered_map<std::string, std::string> query;

      Uri() = default;
      Uri(const basic_uri& uri);
      Uri(const Uri& rhs) = default;
      ~Uri() = default;

      bool HasFragment() const {
        return !fragment.empty();
      }

      bool HasQuery() const {
        return !query.empty();
      }

      std::optional<std::string> GetPathExtension() const {
        const auto dotpos = path.find_first_of('.');
        if(dotpos == std::string::npos)
          return std::nullopt;
        return { path.substr(dotpos + 1) };
      }

      bool HasPathExtension() const {
        const auto dotpos = path.find_first_of('.');
        return dotpos != std::string::npos;
      }

      std::optional<std::string> GetQuery(const std::string& name) const {
        const auto pos = query.find(name);
        if(pos == query.end())
          return std::nullopt;
        return { pos->second };
      }

      Uri& operator=(const Uri& rhs) = default;

      bool operator==(const Uri& rhs) const {
        return scheme == rhs.scheme
            && path == rhs.path;
      }

      bool operator!=(const Uri& rhs) const {
        return scheme != rhs.scheme
            || path != rhs.path;
      }

      friend std::ostream& operator<<(std::ostream& stream, const Uri& rhs) {
        stream << "Uri(";
        stream << "scheme=" << rhs.scheme << ", ";
        stream << "path=" << rhs.path;
        if(rhs.HasQuery()) {
          stream << ", query=[";
          bool first = true;
          for(const auto& [ key, value ] : rhs.query) {
            if(!first)
              stream << ", ";
            stream << key << "=" << value;
            if(first) {
              first = false;
              continue;
            }
          }
          stream << "]";
        }
        if(rhs.HasFragment()) {
          stream << ", fragment=" << rhs.fragment;
        }
        stream << ")";
        return stream;
      }
    };

    static constexpr const uint64_t kDefaultParserBufferSize = 4096;
    static constexpr const uint64_t kDefaultTokenBufferSize = 1024;
    class Parser : public ParserTemplate<kDefaultParserBufferSize, kDefaultTokenBufferSize> {
    public:
      struct Config {
        bool (*OnSchemeParsed)(const Parser* parser, const std::string& value);
        bool (*OnPathParsed)(const Parser* parser, const std::string& value);
        bool (*OnQueryParsed0)(const Parser* parser, const uint64_t idx, const std::string& key);
        bool (*OnQueryParsed1)(const Parser* parser, const uint64_t idx, const std::string& key, const std::string& value);
        bool (*OnFragmentParsed)(const Parser* parser, const std::string& value);
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
      bool Parse();
    };
  }
}

#endif //MCC_URI_H