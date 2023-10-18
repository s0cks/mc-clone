#include <sstream>
#include "mcc/mcc.h"

namespace mcc {
  std::string GetVersion(){
    std::stringstream ss;
    ss << POSEIDON_VERSION_MAJOR << ".";
    ss << POSEIDON_VERSION_MINOR << ".";
    ss << POSEIDON_VERSION_PATCH;
    return ss.str();
  }
}