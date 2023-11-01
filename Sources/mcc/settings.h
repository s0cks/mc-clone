#ifndef MCC_SETTINGS_H
#define MCC_SETTINGS_H

#include <optional>
#include <leveldb/db.h>
#include <gflags/gflags.h>

#include "mcc/gfx.h"
#include "mcc/common.h"

#ifndef MCC_SETTINGS_SYNC_WRITES
#define MCC_SETTINGS_SYNC_WRITES false
#endif //MCC_SETTINGS_SYNC_WRITES

namespace mcc::settings {
  DECLARE_string(settings_dir);

  static constexpr const auto kSyncWrites = MCC_SETTINGS_SYNC_WRITES;

  void Init();

  std::optional<std::string> GetString(const std::string& name);
  bool PutString(const std::string& name, const std::string& value);
  
  std::optional<glm::vec2> GetVec2(const std::string& name);
  bool PutVec2(const std::string& name, const glm::vec2& value);

  std::optional<glm::vec3> GetVec3(const std::string& name);
  bool PutVec3(const std::string& name, const glm::vec3& value);
}

#endif //MCC_SETTINGS_H