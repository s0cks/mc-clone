#ifndef MCC_RESOURCE_H
#define MCC_RESOURCE_H

#include <string>
#include <optional>
#include <glog/logging.h>
#include <leveldb/db.h>
#include <leveldb/options.h>
#include <leveldb/comparator.h>

#include "mcc/flags.h"
#include "mcc/common.h"
#include "mcc/murmur.h"
#include "mcc/relaxed_atomic.h"

namespace mcc {
  struct Resource {
    enum Type : uint8_t {
      kNone = 0,
      kShader,
      kTexture,
      kMaterial,
    };

    static constexpr const auto kMaxKeyLength = 128;
    struct Key {
      uint32_t hash;
      uint8_t data[kMaxKeyLength];

      Key():
        hash(0),
        data() {
        memset(data, '\0', sizeof(data));
      }
      Key(const std::string& value):
        hash(Murmur(value)),
        data() {
        memset(data, '\0', sizeof(data));
        memcpy(data, value.data(), std::min((uint64_t) value.length(), (uint64_t)kMaxKeyLength));
      }
      Key(const uint8_t* value):
        hash(Murmur((const char*) value, kMaxKeyLength)),
        data() {
        memset(data, '\0', sizeof(data));
        memcpy(data, value, sizeof(uint8_t) * kMaxKeyLength);
      }
      Key(const Key& rhs):
        hash(rhs.hash),
        data() {
        memset(data, '\0', sizeof(data));
        memcpy(data, rhs.data, sizeof(uint8_t) * kMaxKeyLength);
      }
      ~Key() = default;

      void operator=(const Key& rhs) {
        hash = rhs.hash;
        memcpy(data, rhs.data, sizeof(uint8_t) * kMaxKeyLength);
      }

      friend std::ostream& operator<<(std::ostream& stream, const Key& rhs) {
        stream << "Key(";
        stream << "hash=" << rhs.hash << ", ";
        stream << "data=" << std::string((const char*) rhs.data);
        stream << ")";
        return stream;
      }
    };

    enum Layout : uint64_t {
      kTypeOffset = 0,
      kTypeSize = sizeof(Type),

      kHashOffset = kTypeOffset + kTypeSize,
      kHashSize = sizeof(uint32_t),

      kDataOffset = kHashOffset + kHashSize,
      kDataSize = kMaxKeyLength,

      kTotalSize = kTypeSize + kHashSize + kDataSize,
    };

  private:
    uint8_t data[kTotalSize];

    void CopyFrom(const uint8_t* src, const uint64_t nbytes) {
      memcpy(data, src, std::min(nbytes, (uint64_t) kTotalSize));
    }
  public:
    Resource():
      data() {
      Clear();
    }
    Resource(const Type t, const std::string& v):
      data() {
      Clear();
      SetType(t);
      SetValue(v);
    }
    Resource(const leveldb::Slice& slice):
      data() {
      Clear();
      CopyFrom((const uint8_t*) slice.data(), slice.size());
    }
    Resource(const Resource& rhs) = default;
    ~Resource() = default;

    Type type() const {
      return *((Type*) &data[kTypeOffset]);
    }

    void SetType(const Type type) {
      memcpy(&data[kTypeOffset], &type, sizeof(Type));
    }

    uint32_t hash() const {
      return *((uint32_t*) &data[kHashOffset]);
    }

    std::string value() const {
      return std::string((const char*) &data[kDataOffset]);
    }

    void SetValue(const std::string& value) {
      const auto hash = Murmur(value);
      memcpy(&data[kHashOffset], &hash, sizeof(uint32_t));
      memset(&data[kDataOffset], 0, kMaxKeyLength);
      memcpy(&data[kDataOffset], value.data(), std::min((uint64_t) value.length(), (uint64_t) kMaxKeyLength));
    }

    void Clear() {
      memset(data, 0, sizeof(uint8_t) * kTotalSize);
    }

    Resource& operator=(const Resource& rhs) = default;

    static inline int
    Compare(const Type& lhs, const Type& rhs) {
      if(lhs < rhs) {
        return -1;
      } else if(lhs > rhs) {
        return +1;
      }
      return 0;
    }

    static inline int
    Compare(const Key& lhs, const Key& rhs) {
      return memcmp(lhs.data, rhs.data, sizeof(uint8_t) * kMaxKeyLength);
    }

    static inline int
    Compare(const Resource& lhs, const Resource& rhs) {
      int result = 0;
      if((result = Compare(lhs.type(), rhs.type())) != 0)
        return result;
      if(lhs.hash() == rhs.hash())
        return 0;
      return memcmp(&lhs.data[kDataOffset], &rhs.data[kDataOffset], kMaxKeyLength);
    }

    bool operator==(const Resource& rhs) const {
      if(Compare(type(), rhs.type()) != 0)
        return false;
      if(hash() != rhs.hash())
        return false;
      return memcmp(&data[kDataOffset], &rhs.data[kDataOffset], kMaxKeyLength) == 0;
    }

    bool operator!=(const Resource& rhs) const {
      if(Compare(type(), rhs.type()) != 0)
        return true;
      if(hash() != rhs.hash())
        return true;
      return memcmp(&data[kDataOffset], &rhs.data[kDataOffset], kMaxKeyLength) != 0;
    }

    bool operator<(const Resource& rhs) const {
      return Compare(*this, rhs) < 0;
    }

    bool operator>(const Resource& lhs) const {
      return Compare(*this, lhs) > 0;
    }

    explicit operator leveldb::Slice() const {
      return leveldb::Slice((const char*) data, kTotalSize);
    }

    friend std::ostream& operator<<(std::ostream& stream, const Resource& rhs) {
      stream << "Resource(";
      stream << "type=" << rhs.type() << ", ";
      stream << "hash=" << rhs.hash() << ", ";
      stream << "value=" << rhs.value();
      stream << ")";
      return stream;
    }
  public:
    static inline Resource
    Material(const std::string& name) {
      return Resource(Type::kMaterial, name);
    }
  };

#define FOR_EACH_RESOURCE_REGISTRY_STATE(V) \
  V(Uninitialized)                       \
  V(Initializing)                        \
  V(Initialized)

  template<typename T>
  struct ResourceReference {
    Resource resource; //TODO: maybe inherit from Resource
    std::string location;

    ResourceReference():
      resource(),
      location() {
    }
    ResourceReference(const Resource& r, const std::string& l):
      resource(r),
      location(l) {
    }
    ResourceReference(const ResourceReference& rhs) = default;
    ~ResourceReference() = default;

    bool valid() const {
      return resource.type() != Resource::kNone
          && FileExists(FLAGS_resources + location);
    }

    std::shared_ptr<T> Load() {
      //TODO: do type checking
      return T::LoadFrom(FLAGS_resources + location);
    }

    ResourceReference& operator=(const ResourceReference& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const ResourceReference& rhs) {
      stream << "ResourceReference(";
      stream << "resource=" << rhs.resource << ", ";
      stream << "location=" << rhs.location;
      stream << ")";
      return stream;
    }
  };

  class ResourceRegistry {
    DEFINE_NON_INSTANTIABLE_TYPE(ResourceRegistry);
  public:
    enum State : uint8_t {
#define DEFINE_STATE(Name) k##Name,
      FOR_EACH_RESOURCE_REGISTRY_STATE(DEFINE_STATE)
#undef DEFINE_STATE
    };

    friend std::ostream& operator<<(std::ostream& stream, const State& rhs) {
      switch(rhs){
#define DEFINE_TOSTRING(Name) \
        case State::k##Name:  \
          return stream << #Name;
        FOR_EACH_RESOURCE_REGISTRY_STATE(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
        default:
          return stream << "<unknown ResourceRegistry::State: " << static_cast<uint8_t>(rhs) << ">";
      }
    }
  public:
    class ResourceComparator : public leveldb::Comparator {
    public:
      ResourceComparator() = default;
      ~ResourceComparator() override = default;

      int Compare(const leveldb::Slice& lhs, const leveldb::Slice& rhs) const override {
        Resource r1(lhs);
        Resource r2(rhs);
        return Resource::Compare(r1, r2);
      }

      const char* Name() const override {
        return "Comparator";
      }

      void FindShortestSeparator(std::string* str, const leveldb::Slice& slice) const override{}
      void FindShortSuccessor(std::string* str) const override{}
    };
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
    static void OnTerminating();

    static void InitializeIndex();
    static void SetState(const State state);
    static bool Put(const Resource k, const std::string& value);
    static bool Delete(const Resource k);
    static std::optional<std::string> GetFilename(const Resource k);
  public:
    static void Init();

    static State GetState();
    static bool Has(const Resource k);

    template<typename T>
    static std::optional<ResourceReference<T>> Get(const Resource k) {
      auto filename = GetFilename(k);
      if(!filename)
        return std::nullopt;
      return std::optional<ResourceReference<T>>{ ResourceReference<T>(k, (*filename)) };
    }
    
#define DEFINE_STATE_CHECK(Name) \
    static inline bool Is##Name() { return GetState() == State::k##Name; }
    FOR_EACH_RESOURCE_REGISTRY_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK
  };
}

#endif //MCC_RESOURCE_H