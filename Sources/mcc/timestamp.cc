#include <iomanip>
#include <sstream>

#include "mcc/timestamp.h"

namespace mcc {
  std::string FormatTimestamp(const Timestamp ts, const std::string format) {
    std::time_t tt = SysClock::to_time_t(ts);
    std::tm tm = *std::gmtime(&tt);
    std::stringstream ss;
    ss << std::put_time(&tm, format.c_str());
    return ss.str();
  }
}