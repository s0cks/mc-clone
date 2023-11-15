#ifndef MCC_RESOURCE_H
#error "Please #include <mcc/resource.h> instead."
#endif //MCC_RESOURCE_H

#ifndef MCC_RESOURCE_TAG_H
#define MCC_RESOURCE_TAG_H

#include <string>
#include <glog/logging.h>
#include <leveldb/slice.h>
#include "mcc/murmur.h"
#include "mcc/resource_type.h"

namespace mcc {
  namespace resource {
    class Tag {
    public:
      static constexpr const auto kMaxKeyLength = 128;
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

      inline void CopyFrom(const uint8_t* src, const uint64_t nbytes) {
        memcpy(data, src, std::min(nbytes, (uint64_t) kTotalSize));
      }

      inline void CopyFrom(const Tag& rhs) {
        return CopyFrom(rhs.data, kTotalSize);
      }
    public:
      Tag():
        data() {
        Clear();
      }
      Tag(const Type t, const std::string& v):
        data() {
        Clear();
        SetType(t);
        SetValue(v);
      }
      Tag(const leveldb::Slice& slice):
        data() {
        Clear();
        CopyFrom((const uint8_t*) slice.data(), slice.size());
      }
      Tag(const Tag& rhs):
        data() {
        Clear();
        CopyFrom(rhs);
      }
      ~Tag() = default;

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

      Tag& operator=(const Tag& rhs) = default;

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
      Compare(const Tag& lhs, const Tag& rhs) {
        int result = 0;
        if((result = Compare(lhs.type(), rhs.type())) != 0)
          return result;
        if(lhs.hash() == rhs.hash())
          return 0;
        return memcmp(&lhs.data[kDataOffset], &rhs.data[kDataOffset], kMaxKeyLength);
      }

      bool operator==(const Tag& rhs) const {
        if(Compare(type(), rhs.type()) != 0)
          return false;
        if(hash() != rhs.hash())
          return false;
        return memcmp(&data[kDataOffset], &rhs.data[kDataOffset], kMaxKeyLength) == 0;
      }

      bool operator!=(const Tag& rhs) const {
        if(Compare(type(), rhs.type()) != 0)
          return true;
        if(hash() != rhs.hash())
          return true;
        return memcmp(&data[kDataOffset], &rhs.data[kDataOffset], kMaxKeyLength) != 0;
      }

      bool operator<(const Tag& rhs) const {
        return Compare(*this, rhs) < 0;
      }

      bool operator>(const Tag& lhs) const {
        return Compare(*this, lhs) > 0;
      }

      explicit operator leveldb::Slice() const {
        return leveldb::Slice((const char*) data, kTotalSize);
      }

      friend std::ostream& operator<<(std::ostream& stream, const Tag& rhs) {
        stream << "Resource(";
        stream << "type=" << rhs.type() << ", ";
        stream << "hash=" << rhs.hash() << ", ";
        stream << "value=" << rhs.value();
        stream << ")";
        return stream;
      }
    public:
      static inline Tag
      Material(const std::string& name) {
        return Tag(Type::kMaterialType, name);
      }

      static inline Tag
      Shader(const std::string& name) {
        return Tag(Type::kShaderType, name);
      }

      static inline Tag
      Texture(const std::string& name) {
        return Tag(Type::kTextureType, name);
      }
    };
  }
}

#endif //MCC_RESOURCE_TAG_H