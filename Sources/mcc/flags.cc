#include "mcc/flags.h"

namespace mcc {
  DEFINE_string(resources, "", "The folder where resources are stored.");

  DEFINE_bool(fullscreen, false, "Whether or not to launch in fullscreen.");
  DEFINE_uint32(width, 1024, "The width of the window.");
  DEFINE_uint32(height, 1024, "The height of the window.");
}