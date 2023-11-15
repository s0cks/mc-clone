#ifndef MCC_RESOURCE_H
#error "Please #include <mcc/resource.h> instead"
#endif //MCC_RESOURCE_H

#ifndef MCC_RESOURCE_REFERENCE_H
#define MCC_RESOURCE_REFERENCE_H

#include "mcc/flags.h"
#include "mcc/resource_tag.h"

namespace mcc::resource {
  template<typename T>
  struct Reference {
    Tag resource; //TODO: maybe inherit from Resource
    std::string location;

    Reference():
      resource(),
      location() {
    }
    Reference(const Tag& r, const std::string& l):
      resource(r),
      location(l) {
    }
    Reference(const Reference& rhs) = default;
    ~Reference() = default;

    bool valid() const {
      return resource.type() != Type::kUnknownType
          && FileExists(FLAGS_resources + location);
    }

    std::shared_ptr<T> Load() {
      //TODO: do type checking
      return T::LoadFrom(location);
    }

    Reference& operator=(const Reference& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const Reference& rhs) {
      stream << "resource::Reference(";
      stream << "resource=" << rhs.resource << ", ";
      stream << "location=" << rhs.location;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_RESOURCE_REFERENCE_H