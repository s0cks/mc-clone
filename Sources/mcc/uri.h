#ifndef MCC_URI_H
#define MCC_URI_H

#include <cstdio>
#include <string>
#include <optional>
#include <fmt/format.h>

#include "mcc/buffer.h"
#include "mcc/murmur.h"
#include "mcc/parser.h"
#include "mcc/common.h"

namespace mcc {
  namespace uri {
    typedef std::string basic_uri;
    
    class Parser;
    struct Uri {
    public:
      std::string scheme;
      std::string path;
      std::string fragment;
      std::unordered_map<std::string, std::string> query;
    private:
      Uri(const std::string& s,
          const std::string& p,
          const std::string& f):
        scheme(s),
        path(p),
        fragment(f) {
      }
      Uri(const std::string& s,
          const std::string& p):
        Uri(s, p, {}) {
      }
    public:
      Uri() = default;
      Uri(const basic_uri& uri);
      Uri(const Uri& rhs) = default;
      ~Uri() = default;

      bool HasScheme() const {
        return !scheme.empty();
      }

      bool HasScheme(const std::string& value) const {
        return HasScheme() && EqualsIgnoreCase(scheme, value);
      }

      bool HasFragment() const {
        return !fragment.empty();
      }

      bool HasQuery() const {
        return !query.empty();
      }

      bool HasExtension() const {
        const auto dotpos = path.find_last_of('.');
        return dotpos != std::string::npos;
      }

      bool HasExtension(const std::string& extension) const {
        const auto dotpos = path.find_last_of('.');
        if(dotpos == std::string::npos)
          return false;
        const auto path_extension = path.substr(extension[0] != '.' ? (dotpos + 1) : dotpos);
        return EqualsIgnoreCase(path_extension, extension);
      }

      std::string GetPathWithoutExtension() const {
        const auto dotpos = path.find_first_of('.');
        if(dotpos != std::string::npos)
          return path.substr(0, path.size() - dotpos);
        return path;
      }

      std::optional<std::string> GetPathExtension() const {
        const auto dotpos = path.find_first_of('.');
        if(dotpos == std::string::npos)
          return std::nullopt;
        return { path.substr(dotpos + 1) };
      }

      std::optional<std::string> GetQuery(const std::string& name) const {
        const auto pos = query.find(name);
        if(pos == query.end())
          return std::nullopt;
        return { pos->second };
      }

      std::string GetParentPath() const {
        const auto slashpos = path.find_last_of('/');
        if(slashpos == std::string::npos)
          return "/";
        return path.substr(0, path.size() - (path.size() - slashpos));
      }

      Uri GetChild(const std::string& child_path) const {
        const auto new_uri = fmt::format("{0:s}://{1:s}/{2:s}", scheme, path, child_path);
        return { new_uri };
      }

      Uri GetSibling(const std::string& sibling_path) const {
        const auto new_uri = fmt::format("{0:s}://{1:s}/{2:s}", scheme, GetParentPath(), sibling_path);
        return { new_uri };
      }

      FILE* OpenFile(const char* mode) const {
        MCC_ASSERT(HasScheme("file"));
        return fopen(path.c_str(), mode);
      }

      inline FILE* OpenFileForReading() const {
        return OpenFile("rb");
      }

      inline FILE* OpenFileForWriting() const {
        return OpenFile("wb");
      }

      Uri& operator=(const Uri& rhs) = default;

      explicit operator std::string() const {
        return fmt::format("{0:s}://{1:s}", scheme, path);
      }

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

    static uri::Uri Shader(const std::string& name) {
      return { fmt::format("shader://{0:s}", name) };
    }

    static inline bool
    IsDirectory(const Uri& uri) {
      return IsDirectory(uri.path);
    }

    static inline bool
    IsDirectory(const std::string& root, const Uri& uri) {
      const auto abs_path = fmt::format("{0:s}/{1:s}", root, uri.path);
      return mcc::IsDirectory((const std::string&) abs_path);
    }

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