#ifndef MCC_URI_H
#define MCC_URI_H

#include <set>
#include <cstdio>
#include <string>
#include <optional>
#include <unordered_set>
#include <fmt/format.h>

#include "mcc/rx.h"
#include "mcc/common.h"

namespace mcc {
  namespace uri {
    typedef std::unordered_map<std::string, std::string> QueryMap;

    void SanitizePath(std::string& value, const bool root = true);
    void SanitizeExtension(std::string& value);

    typedef std::string basic_uri;
    class Parser;
    struct Uri {
    public:
      std::string scheme;
      std::string path;
      std::string fragment;
      QueryMap query;
    private:
      Uri(const std::string& s, const Uri& other):
        scheme(s),
        path(other.path),
        fragment(other.fragment),
        query(other.query) {  
      }
      Uri(const std::string& s, const std::string& root, const Uri& path):
        scheme(s),
        path(fmt::format("{0:s}/{1:s}", root, path.path[0] == '/' ? path.path.substr(1) : path.path)),
        fragment(path.fragment),
        query(path.query) {
      }
    public:
      Uri() = default;
      Uri(const basic_uri& uri);
      Uri(const std::string& s,
          const std::string& p,
          const std::string& f,
          const QueryMap& q):
        scheme(s),
        path(p),
        fragment(f),
        query(q) {
      }
      Uri(const Uri& rhs) = default;
      ~Uri() = default;

      bool HasScheme() const;
      bool HasScheme(const std::string& a) const;
      bool HasScheme(const std::string& a,
                     const std::string& b) const;
      bool HasScheme(const std::string& a,
                     const std::string& b,
                     const std::string& c) const;
      bool HasScheme(const std::string& a,
                     const std::string& b,
                     const std::string& c,
                     const std::string& d) const;
      bool HasScheme(const std::string& a,
                     const std::string& b,
                     const std::string& c,
                     const std::string& d,
                     const std::string& e) const;
      bool HasScheme(const std::string& a,
                     const std::string& b,
                     const std::string& c,
                     const std::string& d,
                     const std::string& e,
                     const std::string& f) const;
      bool HasScheme(const std::unordered_set<std::string>& values) const;
      bool HasScheme(const std::set<std::string>& values) const;

      bool HasFragment() const {
        return !fragment.empty();
      }

      bool HasQuery() const {
        return !query.empty();
      }

      std::string GetExtension() const;
      bool HasExtension() const;
      bool HasExtension(const std::string& a) const;
      bool HasExtension(const std::string& a,
                        const std::string& b) const;
      bool HasExtension(const std::string& a,
                        const std::string& b,
                        const std::string& c) const;
      bool HasExtension(const std::string& a,
                        const std::string& b,
                        const std::string& c,
                        const std::string& d) const;
      bool HasExtension(const std::string& a,
                        const std::string& b,
                        const std::string& c,
                        const std::string& d,
                        const std::string& e) const;
      bool HasExtension(const std::string& a,
                        const std::string& b,
                        const std::string& c,
                        const std::string& d,
                        const std::string& e,
                        const std::string& f) const;
      bool HasExtension(const std::set<std::string>& extensions) const;
      bool HasExtension(const std::unordered_set<std::string>& extensions) const;

      std::string GetResourceName() const {
        const auto slashpos = path.find_last_of('/');
        if(slashpos == std::string::npos)
          return path;
        return path.substr(slashpos + 1);
      }

      std::string GetPathWithoutExtension() const {
        const auto dotpos = path.find_first_of('.');
        if(dotpos != std::string::npos)
          return path.substr(0, path.size() - dotpos);
        return path;
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

      Uri GetParent() const {
        return Uri(scheme, GetParentPath());
      }

      std::string GetChildPath(const std::string& child) const {
        auto child_path = child;
        SanitizePath(child_path, false);
        return fmt::format("{0:s}/{1:s}", path, child_path);
      }

      Uri GetChild(const std::string& child) const {
        return Uri(fmt::format("{0:s}://{1:s}", scheme, GetChildPath(child)));
      }

      std::string GetSiblingPath(const std::string& sibling) const {
        auto sibling_path = sibling;
        SanitizePath(sibling_path, false);
        return fmt::format("{0:s}/{1:s}", GetParentPath(), sibling_path);
      }

      Uri GetSibling(const std::string& sibling) const {
        return Uri(fmt::format("{0:s}://{1:s}", scheme, GetSiblingPath(sibling)));
      }

      Uri ToFileUri(const std::string& root) const {
        return Uri("file", root, *this);
      }

      Uri ToFileUri() const {
        return Uri("file", *this);
      }

      bool IsFile() const {
        return EqualsIgnoreCase(scheme, "file");
      }

      bool IsAbsolutePath() const {
        return IsFile()
            && (FileExists(path));
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

      std::string ToString() const;

      explicit operator std::string() const {
        return fmt::format("{0:s}://{1:s}", scheme, path);
      }

      Uri& operator=(const Uri& rhs) = default;

      bool operator==(const Uri& rhs) const {
        return EqualsIgnoreCase(scheme, rhs.scheme)
            && EqualsIgnoreCase(path, rhs.path);
      }

      bool operator!=(const Uri& rhs) const {
        return !EqualsIgnoreCase(scheme, rhs.scheme)
            || !EqualsIgnoreCase(path, rhs.path);
      }

      friend std::ostream& operator<<(std::ostream& stream, const Uri& rhs) {
        return stream << ((const std::string&) rhs);
      }
    };

    static inline bool
    IsDirectory(const Uri& uri) {
      return mcc::IsDirectory(uri.path);
    }

    static inline bool
    IsDirectory(const std::string& root, const Uri& uri) {
      const auto abs_path = fmt::format("{0:s}/{1:s}", root, uri.path);
      return mcc::IsDirectory((const std::string&) abs_path);
    }

    static inline bool
    FileExists(const uri::Uri& uri) {
      return mcc::FileExists(uri.path);
    }

    bool IsValidUri(const basic_uri& rhs);
    bool IsValidUri(const basic_uri& rhs, const std::string& scheme);
    bool IsValidUri(const basic_uri& rhs, const std::set<std::string>& schemes);
    bool IsValidUri(const basic_uri& rhs, const std::unordered_set<std::string>& schemes);
  }

  static inline uint32_t
  Murmur(const uri::Uri& uri) {
    return Murmur((const std::string&) uri);
  }
}

#endif //MCC_URI_H