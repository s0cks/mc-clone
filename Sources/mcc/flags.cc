#include "mcc/flags.h"

namespace mcc {
  DEFINE_string(resources, "", "The folder where resources are stored.");

  DEFINE_bool(fullscreen, false, "Whether or not to launch in fullscreen.");
  DEFINE_uint32(width, 512, "The width of the window.");
  DEFINE_uint32(height, 512, "The height of the window.");
}