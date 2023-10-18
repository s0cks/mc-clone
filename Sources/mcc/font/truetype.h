#ifndef MCC_FONT_TRUETYPE_H
#define MCC_FONT_TRUETYPE_H

#include "mcc/gfx.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace mcc::font {
  typedef FT_UInt FontSize;
}

#endif //MCC_FONT_TRUETYPE_H