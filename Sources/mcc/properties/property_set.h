#ifndef MCC_PROPERTY_SET_H
#define MCC_PROPERTY_SET_H

#include <unordered_set>

#include "mcc/common.h"
#include "mcc/properties/property.h"

namespace mcc::properties {
  class PropertySet {
    DEFINE_NON_COPYABLE_TYPE(PropertySet); //TODO: make copyable
  protected:
    std::unordered_set<Property*> all_;

    virtual bool Insert(const Property* property);
  public:
    PropertySet() = default;
    virtual ~PropertySet() = default;
    virtual bool Contains(const std::string& name) const;
    virtual bool Remove(const std::string& name);

    inline bool Insert(const std::string& name, const std::string& value) {
      return Insert(StringProperty::New(name, value));
    }

    inline bool Insert(const std::string& name, const uint64_t value) {
      return Insert(LongProperty::New(name, value));
    }

    inline bool Insert(const std::string& name, const double value) {
      return Insert(DoubleProperty::New(name, value));
    }

    inline bool Insert(const std::string& name, const bool value) {
      return Insert(BoolProperty::New(name, value));
    }

    bool Get(const std::string& name, std::string* result) const;
    bool Get(const std::string& name, uint64_t* result) const;
    bool Get(const std::string& name, double* result) const;
    bool Get(const std::string& name, bool* result) const;
  };
}

#endif //MCC_PROPERTY_SET_H