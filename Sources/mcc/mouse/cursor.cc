#include "mcc/mouse/cursor.h"

#include "mcc/thread_local.h"
#include "mcc/file_resolver.h"
#include "mcc/mouse/mouse_flags.h"

#include "mcc/image/image.h"

namespace mcc::mouse {
  static rx::subject<CursorEvent*> events_;

  rx::observable<CursorEvent*> OnCursorEvent() {
    return events_.get_observable();
  }

  void Cursor::Publish(CursorEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

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