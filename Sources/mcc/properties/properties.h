#ifndef MCC_PROPERTIES_H
#define MCC_PROPERTIES_H

#include "mcc/properties/property.h"
#include "mcc/properties/property_set.h"
#include "mcc/properties/properties_parser.h"

#ifdef MCC_DEBUG

#include "mcc/properties/property_set_printer.h"

#endif //MCC_DEBUG

namespace mcc::properties {
  static inline bool
  Parse(FILE* file, PropertySet& props) {
    MCC_ASSERT(file);
    Parser parser(file);
    return parser.ParseProperties(props);
  }

  static inline bool
  Parse(const uri::Uri& uri, PropertySet& props) {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension("properties"));
    if(!StartsWith(uri.path, FLAGS_resources)) {
      const auto normalized = uri.ToFileUri(FLAGS_resources); // TODO: prolly should change ToFileUri to something like Normalize, considering it is already a file uri
      return Parse(normalized, props);
    }
    const auto file = uri.OpenFileForReading();
    if(!file) {
      LOG(ERROR) << "failed to open: " << uri;
      return false;
    }
    return Parse(file, props);
  }
}

#endif //MCC_PROPERTIES_H