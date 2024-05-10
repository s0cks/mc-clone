#ifndef MCC_SHADER_URI_H
#define MCC_SHADER_URI_H

#include <set>
#include "mcc/uri.h"

namespace mcc::shader {
  const std::set<std::string>& GetValidUriSchemes();
  bool IsValidShaderUri(const uri::Uri& uri);
  bool IsValidShaderJsonUri(const uri::Uri& uri);
  void NormalizeShaderUriPath(uri::Uri& uri);
}

#endif //MCC_SHADER_URI_H