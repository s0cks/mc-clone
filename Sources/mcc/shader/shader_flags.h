#ifndef MCC_SHADER_FLAGS_H
#define MCC_SHADER_FLAGS_H

#include <set>
#include "mcc/flags.h"

namespace mcc::shader {
static constexpr const auto kDefaultShaderDirs = "";
DECLARE_string(shader_dirs);

  std::string GetShaderResourcesDirectory();
}

#endif //MCC_SHADER_FLAGS_H