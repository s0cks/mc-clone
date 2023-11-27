#ifndef MCC_RESOURCE_H
#error "Please #include <mcc/resource.h> instead."
#endif//MCC_RESOURCE_H

#ifndef MCC_RESOURCE_REGISTRY_H
#define MCC_RESOURCE_REGISTRY_H

#include <string>
#include <optional>
#include <glog/logging.h>
#include <leveldb/db.h>
#include <leveldb/options.h>
#include <leveldb/comparator.h>
#include "mcc/common.h"
#include "mcc/resource_token.h"
#include "mcc/reference.h"

namespace mcc {
#define FOR_EACH_RESOURCE_REGISTRY_STATE(V) \
  V(Uninitialized)                       \
  V(Initializing)                        \
  V(Initialized)

  namespace resource {
    class Registry {
      friend class ShaderIndexer;
      friend class TextureIndexer;
      friend class MaterialIndexer;
      DEFINE_NON_INSTANTIABLE_TYPE(Registry);
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
            return stream << "<unknown resource::Registry::State: " << static_cast<uint8_t>(rhs) << ">";
        }
      }
    public:
      class TagComparator : public leveldb::Comparator {
      public:
        TagComparator() = default;
        ~TagComparator() override = default;

        int Compare(const leveldb::Slice& lhs, const leveldb::Slice& rhs) const override {
          Tag r1(lhs);
          Tag r2(rhs);
          return Tag::Compare(r1, r2);
        }

        const char* Name() const override {
          return "resource::Registry::TagComparator";
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
      static bool Put(const Tag k, const std::string& value);
      static bool Delete(const Tag k);
      static bool GetFilename(const Tag k, std::string* result);
    public:
      static void Init();

      static State GetState();
      static bool Has(const Tag k);

      static Token Get(const Tag k) {
        std::string filename;
        if(!GetFilename(k, &filename))
          return Token();
        return Token{ Token(k, filename) };
      }
      
  #define DEFINE_STATE_CHECK(Name) \
      static inline bool Is##Name() { return GetState() == State::k##Name; }
      FOR_EACH_RESOURCE_REGISTRY_STATE(DEFINE_STATE_CHECK)
  #undef DEFINE_STATE_CHECK
    };

    template<const Type T>
    class Indexer {
    protected:
      std::string root_;
      std::string current_;

      Indexer(const std::string& root,
              const std::string& current):
        root_(root),
        current_(current) {
      }
      explicit Indexer(const std::string& root):
        root_(root),
        current_(root) {
      }
    public:
      virtual ~Indexer() = default;
      virtual void Index() = 0;
    };

    class MaterialIndexer : public Indexer<kMaterialType> {
      DEFINE_NON_COPYABLE_TYPE(MaterialIndexer);
    public:
      MaterialIndexer(const std::string& root,
                      const std::string& current):
                      Indexer(root, current) {
      }
      MaterialIndexer():
        Indexer(FLAGS_resources + "/materials") {
      }
      ~MaterialIndexer() override = default;

      void Index() override;
    };

    class ShaderIndexer : public Indexer<kShaderType> {
      DEFINE_NON_COPYABLE_TYPE(ShaderIndexer);
    public:
      ShaderIndexer(const std::string& root,
                    const std::string& current):
                    Indexer(root, current) {
      }
      ShaderIndexer():
        Indexer(FLAGS_resources + "/shaders") {
      }
      ~ShaderIndexer() override = default;

      void Index() override;
    };

    class TextureIndexer : public Indexer<kTextureType> {
      DEFINE_NON_COPYABLE_TYPE(TextureIndexer);
    public:
      TextureIndexer(const std::string& root,
                    const std::string& current):
                    Indexer(root, current) {
      }
      TextureIndexer():
        Indexer(FLAGS_resources + "/textures") {
      }
      ~TextureIndexer() override = default;

      void Index() override;
    };
  }
}

#endif //MCC_RESOURCE_REGISTRY_H