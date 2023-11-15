#include <filesystem>

#include "mcc/resource.h"
#include "mcc/thread_local.h"
#include "mcc/engine/engine.h"
#include "mcc/relaxed_atomic.h"

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

    {
      DLOG(INFO) << "indexing materials....";
      MaterialIndexer indexer;
      indexer.Index();
    }

    {
      DLOG(INFO) << "indexing shaders....";
      ShaderIndexer indexer;
      indexer.Index();
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
      DLOG(INFO) << ref;
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

  bool Registry::GetFilename(const Tag k, std::string* result) {
    READ_OPTIONS;
    leveldb::Status status;
    if(!(status = GetIndex()->Get(readOpts, (const leveldb::Slice&) k, result)).ok()) {
      if(status.IsNotFound())
        return false;
      DLOG(ERROR) << "couldn't get " << k << ": " << status.ToString();
      return false;
    }
    return true;
  }

  void MaterialIndexer::Index() {
    DLOG(INFO) << "indexing directory " << current_ << "....";
    for(const auto& entry : std::filesystem::directory_iterator(current_)) {
      const auto path = std::string(entry.path());
      const auto relative = path.substr(root_.length() + 1);
      const auto name = relative.substr(0, relative.find_last_of("."));
      if(entry.is_directory()) {
        if(FileExists(path + "/material.json")) {
          Registry::Put(Tag::Material(name), path);
          continue;
        }

        MaterialIndexer indexer(root_, path);
        indexer.Index();
      }
    }
  }

  void ShaderIndexer::Index() {
    DLOG(INFO) << "indexing directory " << current_ << "....";
    for(const auto& entry : std::filesystem::directory_iterator(current_)) {
      const auto path = std::string(entry.path());
      const auto relative = path.substr(root_.length() + 1);
      const auto name = relative.substr(0, relative.find_last_of("."));
      if(entry.is_directory()) {
        if(FileExists(path + "/shader.json")) {
          Registry::Put(Tag::Shader(name), path);
          continue;
        }

        ShaderIndexer indexer(root_, path);
        indexer.Index();
      } else if(EndsWith(relative, ".vs") || EndsWith(relative, ".vsh")) {
        //TODO: elegantly determine fragment shader location
        Registry::Put(Tag::Shader(name), path);
        continue;
      }
    }
  }
}