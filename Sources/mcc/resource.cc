#include "mcc/resource.h"

#include "mcc/flags.h"
#include "mcc/thread_local.h"
#include "mcc/engine/engine.h"

#include "mcc/material.h"

namespace mcc {
  static RelaxedAtomic<ResourceRegistry::State> state_(ResourceRegistry::kUninitialized);
  static ThreadLocal<leveldb::DB> index_;

  static inline leveldb::DB*
  GetIndex() {
    return index_.Get();
  }

  void ResourceRegistry::InitializeIndex() {
    if(!ResourceRegistry::IsUninitialized()) {
      DLOG(ERROR) << "cannot re-initialize the ResourceRegistry.";
      return;
    }

    SetState(ResourceRegistry::kInitializing);
    const auto resources = FLAGS_resources;
    if(!FileExists(resources)) {
      LOG(FATAL) << "cannot initialize the ResourceRegistry in non-existent directory: " << resources;
      return;
    }

    const auto index = resources + "/index";
    const auto generated = !FileExists(index);

    leveldb::Options options;
    options.create_if_missing = true;
    options.comparator = new ResourceRegistry::ResourceComparator();
    leveldb::DB* db = nullptr;
    leveldb::Status status;
    if(!(status = leveldb::DB::Open(options, index, &db)).ok()) {
      DLOG(ERROR) << "failed to open ResourceRegistry index from " << index << ", delete and create a new one.";
      //TODO: delete
      return;
    }
    index_.Set(db);

    if(generated) {
      //TODO: auto generate index
      ResourceRegistry::Put(Resource::Material("fabrics/leather_black"), "/materials/fabrics/leather_black");
      ResourceRegistry::Put(Resource::Material("fabrics/soft_blanket"), "/materials/fabrics/soft_blanket");
      ResourceRegistry::Put(Resource::Material("floors/laminate_brown"), "/materials/floors/laminate_brown");
      ResourceRegistry::Put(Resource::Material("floors/old_wood"), "/materials/floors/old_wood");
      ResourceRegistry::Put(Resource::Material("stones/broken_concrete1"), "/materials/stones/broken_concrete_1");
    }
  }

  void ResourceRegistry::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Engine::OnTerminating(&OnTerminating);
  }

  void ResourceRegistry::OnPreInit() {
    InitializeIndex();
  }

  void ResourceRegistry::OnInit() {
    
  }

  void ResourceRegistry::OnPostInit() {
    {
      const auto ref = Get<Material>(Resource::Material("floors/laminate_brown"));
      DLOG(INFO) << (*ref);
    }
  }

  void ResourceRegistry::OnTerminating() {
    delete index_.Get();
    SetState(ResourceRegistry::kUninitialized);
  }

  ResourceRegistry::State ResourceRegistry::GetState() {
    return (ResourceRegistry::State) state_;
  }

  void ResourceRegistry::SetState(const State state) {
    state_ = state;
  }

#define NAMED_READ_OPTIONS(Name) \
  leveldb::ReadOptions Name;

#define READ_OPTIONS \
  NAMED_READ_OPTIONS(readOpts)

#define NAMED_WRITE_OPTIONS(Name) \
  leveldb::WriteOptions Name;     \
  Name.sync = true;

#define WRITE_OPTIONS             \
  NAMED_WRITE_OPTIONS(writeOpts)

  bool ResourceRegistry::Put(const Resource k, const std::string& value) {
    WRITE_OPTIONS;
    leveldb::Status status;
    if(!(status = GetIndex()->Put(writeOpts, (const leveldb::Slice&) k, value)).ok()) {
      DLOG(ERROR) << "cannot index " << k << ": " << status.ToString();
      return false;
    }
    DLOG(INFO) << "indexed " << k << " to " << value;
    return true;
  }

  bool ResourceRegistry::Delete(const Resource k) {
    WRITE_OPTIONS;
    leveldb::Status status;
    if(!(status = GetIndex()->Delete(writeOpts, (const leveldb::Slice&) k)).ok()) {
      DLOG(ERROR) << "cannot delete " << k << ": " << status.ToString();
      return false;
    }
    DLOG(INFO) << "deleted " << k;
    return true;
  }

  bool ResourceRegistry::Has(const Resource k) {
    std::string filename;
    READ_OPTIONS;
    leveldb::Status status;
    if(!(status = GetIndex()->Get(readOpts, (const leveldb::Slice&) k, &filename)).ok()) {
      if(status.IsNotFound())
        return false;
      DLOG(ERROR) << "couldn't get " << k << ": " << status.ToString();
      return false;
    }
    return true;
  }

  std::optional<std::string> ResourceRegistry::GetFilename(const Resource k) {
    std::string filename;
    READ_OPTIONS;
    leveldb::Status status;
    if(!(status = GetIndex()->Get(readOpts, (const leveldb::Slice&) k, &filename)).ok()) {
      if(status.IsNotFound())
        return std::nullopt;
      DLOG(ERROR) << "couldn't get " << k << ": " << status.ToString();
      return std::nullopt;
    }
    return std::optional<std::string>{filename};
  }
}