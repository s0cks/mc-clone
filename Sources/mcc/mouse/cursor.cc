#include "mcc/mouse/cursor.h"

#include "mcc/thread_local.h"
#include "mcc/file_resolver.h"
#include "mcc/mouse/mouse_flags.h"

#include "mcc/image/image.h"

namespace mcc::mouse {
  class AvailableCursorPrinter {
  protected:
    std::string root_;
  public:
    explicit AvailableCursorPrinter(const std::string& root):
      root_(root) {
    }
    ~AvailableCursorPrinter() = default;

    const std::string& GetRoot() const {
      return root_;
    }

    void PrintAll() {
      for(const auto& entry : fs::recursive_directory_iterator(GetRoot())) {

      }
    }
  };

  rx::observable<std::string> ListAvailableCursors() {
    return rx::observable<>::create<std::string>([](rx::subscriber<std::string> s) {
      s.on_next("default");
      const auto dir = GetCursorsDir();
      fs::ListFilesInDirectory<true>(dir)
        .filter(fs::FilterByExtension(img::GetValidExtensions()))
        .map([&dir](const fs::directory_entry& entry) {
          const auto& path = (const std::string&) entry.path();
          if(StartsWith(path, dir))
            return path.substr(dir.length() + 1);
          return path;
        })
        .as_blocking()
        .subscribe([&s](const std::string& next) {
          s.on_next(next);
        });
      s.on_completed();
    });
  }
}