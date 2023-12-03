#ifndef MCC_FILE_RESOLVER_H
#define MCC_FILE_RESOLVER_H

#include <string>
#include <optional>
#include <filesystem>
#include <functional>
#include <unordered_set>
#include <glog/logging.h>

#include "mcc/predicate.h"

namespace mcc {
  template<typename Result>
  class Resolver {
  protected:
    Resolver() = default;
  public:
    virtual ~Resolver() = default;
    virtual std::optional<Result> Resolve() const = 0;
  };

  typedef std::function<bool(const std::filesystem::directory_entry&)> PathPredicate;

  static inline std::optional<std::string>
  GetExtension(const std::string& filename) {
    const auto dotpos = filename.find_first_of('.');
    if(dotpos == std::string::npos)
      return std::nullopt;
    return { filename.substr(dotpos) };
  }

  static inline PathPredicate
  HasExtension(const std::string& extension) {
    return [&](const std::filesystem::directory_entry& e) {
      if(!e.is_regular_file())
        return false;
      return GetExtension(e.path()) == extension;
    };
  }

  static inline PathPredicate
  HasExtension(const std::unordered_set<std::string>& extensions) {
    return [&](const std::filesystem::directory_entry& e) {
      if(!e.is_regular_file())
        return false;
      const auto extension = GetExtension(e.path());
      if(!extension)
        return false;
      const auto pos = extensions.find((*extension));
      return pos != extensions.end();
    };
  }

  template<const bool Recursive, typename Iterator>
  class PathResolverTemplate : public Resolver<std::string> {
  protected:
    std::string root_;
    PathPredicate predicate_;
  public:
    PathResolverTemplate(const std::string& root, PathPredicate predicate):
      Resolver<std::string>(),
      root_(root),
      predicate_(predicate) {
    }
    ~PathResolverTemplate() override = default;

    std::string root() const {
      return root_;
    }

    PathPredicate predicate() const {
      return predicate_;
    }

    bool IsRecursive() const {
      return Recursive;
    }

    std::optional<std::string> Resolve() const override {
      for(const auto& e : Iterator(root_)) {
        if(predicate_(e))
          return { e.path() };
      }
      return std::nullopt;
    }
  };

  typedef PathResolverTemplate<false, std::filesystem::directory_iterator> PathResolver;
  typedef PathResolverTemplate<true, std::filesystem::recursive_directory_iterator> RecursivePathResolver;
}

#endif //MCC_FILE_RESOLVER_H