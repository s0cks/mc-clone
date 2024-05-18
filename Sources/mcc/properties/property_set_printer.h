#ifndef MCC_PROPERTY_SET_PRINTER_H
#define MCC_PROPERTY_SET_PRINTER_H

#include <glog/logging.h>
#include "mcc/properties/property_set.h"

namespace mcc::properties {
  class PropertySetPrinter : public PropertyVisitor {
  protected:
    google::LogSeverity severity_;
  public:
    explicit PropertySetPrinter(const google::LogSeverity severity):
      PropertyVisitor(),
      severity_(severity) {
    }
    ~PropertySetPrinter() override = default;

    google::LogSeverity GetSeverity() const {
      return severity_;
    }
    
    bool Visit(Property* prop) override {
      MCC_ASSERT(prop);
      LOG_AT_LEVEL(GetSeverity()) << " - " << prop->ToString();
      return true;
    }
  public:
    template<const google::LogSeverity Severity = google::INFO>
    static inline bool
    Print(const PropertySet& properties) {
      PropertySetPrinter printer(Severity);
      return properties.VisitAllProperties(&printer);
    }
  };
}

#endif //MCC_PROPERTY_SET_PRINTER_H