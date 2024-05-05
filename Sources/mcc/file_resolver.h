#ifndef MCC_FILE_RESOLVER_H
#define MCC_FILE_RESOLVER_H

#include <string>
#include <optional>
#include <filesystem>
#include <functional>
#include <fmt/format.h>
#include <unordered_set>
#include <glog/logging.h>

#include "mcc/rx.h"
#include "mcc/common.h"

namespace mcc {
  namespace fs {
    using namespace std::filesystem;

    static inline bool
    IsRegularFile(const directory_entry& entry) {
      return entry.is_regular_file();
    }

    static inline bool
    IsDirectory(const directory_entry& entry) {
      return entry.is_directory();
    }

    static inline std::string
    ToPath(const directory_entry& entry) {
      return (const std::string&) entry.path();
    }

    static inline rx::observable<directory_entry>
    ListAllInDirectory(const std::string& dir) {
      if(!FileExists(dir)) {
        const auto err = fmt::format("cannot ls directory {0:s}, directory doesn't exist", dir);
        return rx::observable<>::error<directory_entry>(std::runtime_error(err));
      }
      return rx::observable<>::create<directory_entry>([dir](rx::subscriber<directory_iterator::value_type> s) {
        for(const auto& entry : directory_iterator(dir))
          s.on_next(entry);
        s.on_completed();
      });
    }

    static inline rx::observable<directory_entry>
    ListFilesInDirectory(const std::string& dir) {
      return ListAllInDirectory(dir)
        .filter(IsRegularFile);
    }

    static inline rx::observable<directory_entry>
    ListDirsInDirectory(const std::string& dir) {
      return ListAllInDirectory(dir)
        .filter(IsDirectory);
    }
  }
}

#endif //MCC_FILE_RESOLVER_H