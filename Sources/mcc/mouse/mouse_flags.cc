#include "mcc/mouse/mouse_flags.h"

namespace mcc::mouse {
DEFINE_bool(mouse_visible, kDefaultMouseVisible, "Whether or not the cursor is visible");
DEFINE_bool(capture_mouse, kDefaultCaptureMouse, "Whether or not to constrain the cursor to the window.");
DEFINE_string(cursors_dir, kDefaultCursorsDir, "The directory to load mouse cursors from.");
}