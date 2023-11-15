#include "mcc/resource.h"

namespace mcc::resource {
  static RelaxedAtomic<Registry::State> state_(Registry::kUninitialized);
  static ThreadLocal<leveldb::DB> index_;

  static inline leveldb::DB*
  GetIndex() {
    return index_.Get();
  }

  void Registry::InitializeIndex() {
    if(!Registry::IsUninitialized()) {
      DLOG(ERROR) << "cannot re-initialize the resource::Registry.";
      return;
    }

    SetState(Registry::kInitializing);
    const auto resources = FLAGS_resources;
    if(!FileExists(resources)) {
      LOG(FATAL) << "cannot initialize the resource::Registry in non-existent directory: " << resources;
      return;
    }

    const auto index = resources + "/index";
    const auto generated = !FileExists(index);

    leveldb::Options options;
    options.create_if_missing = true;
    options.comparator = new Registry::TagComparator();
    leveldb::DB* db = nullptr;
    leveldb::Status status;
    if(!(status = leveldb::DB::Open(options, index, &db)).ok()) {
      DLOG(ERROR) << "failed to open resource::Registry index from " << index << ", delete and create a new one.";
      //TODO: delete
      return;
    }
    index_.Set(db);

    if(generated) {
      //TODO: auto generate index
      Registry::Put(Tag::Material("fabrics/leather_black"), "/materials/fabrics/leather_black");
      Registry::Put(Tag::Material("fabrics/soft_blanket"), "/materials/fabrics/soft_blanket");
      Registry::Put(Tag::Material("floors/laminate_brown"), "/materials/floors/laminate_brown");
      Registry::Put(Tag::Material("floors/old_wood"), "/materials/floors/old_wood");
      Registry::Put(Tag::Material("stones/broken_concrete1"), "/materials/stones/broken_concrete_1");
    }
  }

  void Registry::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Engine::OnTerminating(&OnTerminating);
  }

  void Registry::OnPreInit() {
    InitializeIndex();
  }

  void Registry::OnInit() {
    
  }

  void Registry::OnPostInit() {
    {
      const auto ref = Get<Material>(Tag::Material("floors/laminate_brown"));
      DLOG(INFO) << (*ref);
    }
  }

  void Registry::OnTerminating() {
    delete index_.Get();
    SetState(Registry::kUninitialized);
  }

  Registry::State Registry::GetState() {
    return (Registry::State) state_;
  }

  void Registry::SetState(const State state) {
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

  bool Registry::Put(const Tag k, const std::string& value) {
    WRITE_OPTIONS;
    leveldb::Status status;
    if(!(status = GetIndex()->Put(writeOpts, (const leveldb::Slice&) k, value)).ok()) {
      DLOG(ERROR) << "cannot index " << k << ": " << status.ToString();
      return false;
    }
    DLOG(INFO) << "indexed " << k << " to " << value;
    return true;
  }

  bool Registry::Delete(const Tag k) {
    WRITE_OPTIONS;
    leveldb::Status status;
    if(!(status = GetIndex()->Delete(writeOpts, (const leveldb::Slice&) k)).ok()) {
      DLOG(ERROR) << "cannot delete " << k << ": " << status.ToString();
      return false;
    }
    DLOG(INFO) << "deleted " << k;
    return true;
  }

  bool Registry::Has(const Tag k) {
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

  std::optional<std::string> Registry::GetFilename(const Tag k) {
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