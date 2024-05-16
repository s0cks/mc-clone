#ifndef MCC_PROPERTY_SET_H
#define MCC_PROPERTY_SET_H

#include <map>

#include "mcc/common.h"
#include "mcc/properties/property.h"

namespace mcc::properties {
  class PropertySet {
    DEFINE_NON_COPYABLE_TYPE(PropertySet); //TODO: make copyable
  protected:
    std::map<std::string, Property*> all_;

    virtual bool Insert(Property* property) {
      const auto result = all_.insert({ property->GetName(), property });
      return result.second;
    }
  public:
    PropertySet() = default;
    virtual ~PropertySet() = default;
    
    virtual bool Contains(const std::string& name) const {
      const auto pos = all_.find(name);
      return pos != std::end(all_);
    }

    virtual bool Remove(const std::string& name) {
      const auto removed = all_.erase(name);
      return removed == 1;
    }

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

    bool Get(const std::string& name, std::string* result) const {
      const auto pos = all_.find(name);
      if(pos == std::end(all_)) {
        (*result) = nullptr;
        return false;
      }
      const auto& prop = pos->second;
      if(!prop->IsString()) {
        DLOG(WARNING) << prop->ToString() << " is not of type String.";
        (*result) = nullptr;
        return false;
      }
      (*result) = prop->AsString()->GetValue();
      return true;
    }
    
    bool Get(const std::string& name, uint64_t* result) const {
      const auto pos = all_.find(name);
      if(pos == std::end(all_)) {
        (*result) = 0;
        return false;
      }
      const auto& prop = pos->second;
      if(!prop->IsString()) {
        DLOG(WARNING) << prop->ToString() << " is not of type Long.";
        (*result) = 0;
        return false;
      }
      (*result) = prop->AsLong()->GetValue();
      return true;
    }

    bool Get(const std::string& name, double* result) const {
      const auto pos = all_.find(name);
      if(pos == std::end(all_)) {
        (*result) = 0.0;
        return false;
      }
      const auto& prop = pos->second;
      if(!prop->IsDouble()) {
        DLOG(WARNING) << prop->ToString() << " is not of type Double.";
        (*result) = 0.0;
      }
      (*result) = prop->AsDouble()->GetValue();
      return true;
    }

    bool Get(const std::string& name, bool* result) const {
      const auto pos = all_.find(name);
      if(pos == std::end(all_)) {
        (*result) = false;
        return false;
      }
      const auto& prop = pos->second;
      if(!prop->IsString()) {
        DLOG(WARNING) << prop->ToString() << " is not of type Bool.";
        (*result) = false;
        return false;
      }
      (*result) = prop->AsBool()->GetValue();
      return true;
    }
  };
}

#endif //MCC_PROPERTY_SET_H