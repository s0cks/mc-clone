#include <string>
#include <unordered_set>

#include "mcc/uri.h"
#include "mcc/uri_parser.h"

namespace mcc::uri {
  void SanitizeExtension(std::string& extension) {
    if(extension[0] == '.')
      extension = extension.substr(1);
    ToLowercase(extension);
  }

  void SanitizePath(std::string& path, const bool root) {
    if(root && path[0] != '/')
      path = '/' + path;
    else if(!root && path[0] == '/')
      path = path.substr(1);
  }

  static inline bool
  OnSchemeParsed(const Parser* parser, const char* scheme, const uint64_t scheme_len) {
    auto uri = (Uri*)parser->data();
    uri->scheme = std::string(scheme, scheme_len);
    return true;
  }

  static inline bool
  OnPathParsed(const Parser* parser, const char* path, const uint64_t path_len) {
    auto uri = (Uri*)parser->data();
    uri->path = std::string(path, path_len);
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
  OnFragmentParsed(const Parser* parser, const char* fragment, const uint64_t fragment_len) {
    auto uri = (Uri*)parser->data();
    uri->fragment = std::string(fragment, fragment_len);
    return true;
  }

  Uri::Uri(const basic_uri& uri):
    scheme(),
    path() {
    Parser::Config config = {
      .OnParseScheme = &OnSchemeParsed,
      .OnParsePath = &OnPathParsed,
      .OnParseQuery0 = &OnQueryParsed0,
      .OnParseQuery1 = &OnQueryParsed1,
      .OnParseFragment = &OnFragmentParsed,
    };
    Parser parser(config, uri, this);
    LOG_IF(ERROR, !parser.Parse()) << "failed to parse uri: " << uri;
  }

  bool Uri::HasScheme() const {
    return !scheme.empty();
  }

  bool Uri::HasScheme(const std::string& a) const {
    return HasScheme() 
        && EqualsIgnoreCase(scheme, a);
  }

  bool Uri::HasScheme(const std::string& a,
                      const std::string& b) const {
    return HasScheme()
        && (EqualsIgnoreCase(scheme, a)
        || EqualsIgnoreCase(scheme, b));
  }

  bool Uri::HasScheme(const std::string& a,
                      const std::string& b,
                      const std::string& c) const {
    return HasScheme()
        && (EqualsIgnoreCase(scheme, a)
        || EqualsIgnoreCase(scheme, b)
        || EqualsIgnoreCase(scheme, c));
  }

  bool Uri::HasScheme(const std::string& a,
                      const std::string& b,
                      const std::string& c,
                      const std::string& d) const {
    return HasScheme()
        && (EqualsIgnoreCase(scheme, a)
        || EqualsIgnoreCase(scheme, b)
        || EqualsIgnoreCase(scheme, c)
        || EqualsIgnoreCase(scheme, d));
  }

  bool Uri::HasScheme(const std::string& a,
                      const std::string& b,
                      const std::string& c,
                      const std::string& d,
                      const std::string& e) const {
    return HasScheme()
        && (EqualsIgnoreCase(scheme, a)
        || EqualsIgnoreCase(scheme, b)
        || EqualsIgnoreCase(scheme, c)
        || EqualsIgnoreCase(scheme, d)
        || EqualsIgnoreCase(scheme, e));
  }

  bool Uri::HasScheme(const std::string& a,
                      const std::string& b,
                      const std::string& c,
                      const std::string& d,
                      const std::string& e,
                      const std::string& f) const {
    return HasScheme()
        && (EqualsIgnoreCase(scheme, a)
        || EqualsIgnoreCase(scheme, b)
        || EqualsIgnoreCase(scheme, c)
        || EqualsIgnoreCase(scheme, d)
        || EqualsIgnoreCase(scheme, e)
        || EqualsIgnoreCase(scheme, f));
  }

  bool Uri::HasScheme(const std::unordered_set<std::string>& values) const {
    if(!HasScheme())
      return false;
    const auto pos = values.find(scheme);
    return pos != values.end();
  }

  bool Uri::HasScheme(const std::set<std::string>& values) const {
    if(!HasScheme())
      return false;
    const auto pos = values.find(scheme);
    return pos != values.end();
  }

  std::string Uri::GetExtension() const {
    const auto dotpos = path.find_last_of('.');
    if(dotpos == std::string::npos)
      return {};
    return path.substr(dotpos + 1);
  }

  bool Uri::HasExtension() const {
    const auto dotpos = path.find_last_of('.');
    return dotpos != std::string::npos;
  }

  bool Uri::HasExtension(const std::string& a) const {
    auto extension = a;
    SanitizeExtension(extension);
    return EqualsIgnoreCase(extension, GetExtension());
  }

  bool Uri::HasExtension(const std::string& a,
                         const std::string& b) const {
    auto e1 = a;
    SanitizeExtension(e1);
    auto e2 = b;
    SanitizeExtension(e2);
    auto extension = GetExtension();
    SanitizeExtension(extension);
    return EqualsIgnoreCase(extension, e1)
        || EqualsIgnoreCase(extension, e2);
  }

  bool Uri::HasExtension(const std::string& a,
                         const std::string& b,
                         const std::string& c) const {
    auto e1 = a;
    SanitizeExtension(e1);
    auto e2 = b;
    SanitizeExtension(e2);
    auto e3 = c;
    SanitizeExtension(e3);
    auto extension = GetExtension();
    SanitizeExtension(extension);
    return EqualsIgnoreCase(extension, e1)
        || EqualsIgnoreCase(extension, e2)
        || EqualsIgnoreCase(extension, e3);
  }

  bool Uri::HasExtension(const std::string& a,
                         const std::string& b,
                         const std::string& c,
                         const std::string& d) const {
    auto e1 = a;
    SanitizeExtension(e1);
    auto e2 = b;
    SanitizeExtension(e2);
    auto e3 = c;
    SanitizeExtension(e3);
    auto e4 = d;
    SanitizeExtension(e3);
    auto extension = GetExtension();
    SanitizeExtension(extension);
    return EqualsIgnoreCase(extension, e1)
        || EqualsIgnoreCase(extension, e2)
        || EqualsIgnoreCase(extension, e3)
        || EqualsIgnoreCase(extension, e4);
  }

  bool Uri::HasExtension(const std::string& a,
                         const std::string& b,
                         const std::string& c,
                         const std::string& d,
                         const std::string& e) const {
    auto e1 = a;
    SanitizeExtension(e1);
    auto e2 = b;
    SanitizeExtension(e2);
    auto e3 = c;
    SanitizeExtension(e3);
    auto e4 = d;
    SanitizeExtension(e4);
    auto e5 = e;
    SanitizeExtension(e5);
    auto extension = GetExtension();
    SanitizeExtension(extension);
    return EqualsIgnoreCase(extension, e1)
        || EqualsIgnoreCase(extension, e2)
        || EqualsIgnoreCase(extension, e3)
        || EqualsIgnoreCase(extension, e4)
        || EqualsIgnoreCase(extension, e5);
  }

  bool Uri::HasExtension(const std::string& a,
                         const std::string& b,
                         const std::string& c,
                         const std::string& d,
                         const std::string& e,
                         const std::string& f) const {
    auto e1 = a;
    SanitizeExtension(e1);
    auto e2 = b;
    SanitizeExtension(e2);
    auto e3 = c;
    SanitizeExtension(e3);
    auto e4 = d;
    SanitizeExtension(e4);
    auto e5 = e;
    SanitizeExtension(e5);
    auto e6 = f;
    SanitizeExtension(e6);
    auto extension = GetExtension();
    SanitizeExtension(extension);
    return EqualsIgnoreCase(extension, e1)
        || EqualsIgnoreCase(extension, e2)
        || EqualsIgnoreCase(extension, e3)
        || EqualsIgnoreCase(extension, e4)
        || EqualsIgnoreCase(extension, e5)
        || EqualsIgnoreCase(extension, e6);
  }

  bool Uri::HasExtension(const std::set<std::string>& extensions) const {
    auto extension = GetExtension();
    SanitizeExtension(extension);
    const auto pos = extensions.find(extension);
    return pos != extensions.end();
  }

  bool Uri::HasExtension(const std::unordered_set<std::string>& extensions) const {
    auto extension = GetExtension();
    SanitizeExtension(extension);
    const auto pos = extensions.find(extension);
    return pos != extensions.end();
  }

  std::string Uri::ToString() const {
    std::stringstream stream;
    stream << "Uri(";
    stream << "scheme=" << scheme << ", ";
    stream << "path=" << path;
    if(HasQuery()) {
      stream << ", query=[";
      bool first = true;
      for(const auto& [ key, value ] : query) {
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
    if(HasFragment())
      stream << ", fragment=" << fragment;
    stream << ")";
    return stream.str();
  }
}