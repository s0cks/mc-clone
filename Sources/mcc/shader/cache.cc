#include <unordered_map>
#include "mcc/shader/cache.h"

#include "mcc/shader/builder.h"
#include "mcc/shader/compiler.h"
#include "mcc/relaxed_atomic.h"

#include "mcc/murmur.h"

namespace mcc::shader {
  DEFINE_string(shader_cache_dir, kDefaultShaderCacheDir, "The directory for all stored shaders.");
  DEFINE_uint32(shader_cache_size, kDefaultShaderCacheSize, "The size of the shader cache.");
  DEFINE_uint32(shader_cache_buckets, kDefaultShaderCacheBuckets, "The number of buckets for the shader cache to use.");

  struct Node {
    Node* next;
    Node* previous;
    uint32_t key;
    Shader value;
  };

  static Node* head_;
  static Node* tail_;
  static Node** nodes_;
  static uint32_t size_;

  static inline uint32_t
  GetNumberOfBuckets() {
    return FLAGS_shader_cache_buckets;
  }

  static inline uint32_t
  GetCapacity() {
    return FLAGS_shader_cache_size;
  }

  static inline uint32_t
  GetKey(const std::string& k) {
    return Murmur(k.data(), k.size());
  }

  static inline uint32_t
  GetIndex(const uint32_t k) {
    return k % GetNumberOfBuckets();
  }

  static inline void
  Initialize() {
    const auto num_buckets = GetNumberOfBuckets();
    const auto nodes = (Node**)malloc(sizeof(Node*) * num_buckets);
    LOG_IF(FATAL, !nodes) << "failed to initialize shader cache nodes.";
    memset(nodes, 0, sizeof(Node*) * num_buckets);
    size_ = 0;
    head_ = nullptr;
    tail_ = nullptr;
    nodes_ = nodes;
  }

  static inline void
  RemoveNode(Node* node) {
    if(node == head_) {
      head_ = node->next;
    } else if(node == tail_) {
      tail_ = node->previous;
    } else {
      const auto previous = node->previous;
      const auto next = node->next;
      if(previous)
        previous->next = next;
      if(next)
        next->previous = previous;
    }
    size_ -= 1;
  }

  static inline void
  PushFront(Node* node) {
    if(head_ == nullptr) {
      head_ = tail_ = node;
    } else {
      node->next = head_;
      if(head_)
        head_->previous = node;
      head_ = node;
    }
    size_ += 1;
  }

  void Cache::Init() {
    Initialize();
  }

  uint32_t Cache::GetSize() {
    return (uint32_t) size_;
  }

  Shader Cache::Get(const std::string& name, CacheLoader* loader) {
    MCC_ASSERT(loader != nullptr);
    const auto key = GetKey(name);
    const auto index = GetIndex(key);
    auto node = nodes_[index];
    while(node != nullptr) {
      if(node->key == key) {
        RemoveNode(node);
        PushFront(node);
        return node->value;
      }
    }

    if((GetSize() + 1) >= GetCapacity()) {
      const auto tail = tail_;
      RemoveNode(tail_);
      delete tail;
    }

    const auto value = loader->Load(name);
    if(!value.IsCompiled())
      return value;

    node = new Node();
    node->key = key;
    node->value = value;
    PushFront(node);
    return value;
  }

  static inline std::string
  GetShaderFilename(const std::string& name, const std::string& extension) {
    const auto root = Cache::GetCacheDirectory();
    auto filename = name;
    if(!StartsWith(filename, root))
      filename = root + "/" + name;
    if(!EndsWith(filename, extension))
      filename = filename + extension;
    return filename;
  }

  static inline std::string
  GetVertexShaderFilename(const std::string& name) {
    return GetShaderFilename(name, kVertexShaderExtension);
  }

  static inline std::string
  GetFragmentShaderFilename(const std::string& name) {
    return GetShaderFilename(name, kFragmentShaderExtension);
  }

  Shader BasicCacheLoader::Load(const std::string& k) const {
    ProgramBuilder builder;
    {
      const auto filename = GetVertexShaderFilename(k);
      DLOG(INFO) << "compiling vertex shader for " << k << " from " << filename;
      const auto source = Source::FromFile(filename);
      LOG_IF(FATAL, !source) << "failed to fetch vertex shader source for " << k << " from " << filename;
      const auto result = builder.AttachVertexShader(source);
      LOG_IF(FATAL, !result.success()) << "failed to compile vertex shader: " << result;
    }

    {
      const auto filename = GetFragmentShaderFilename(k);
      DLOG(INFO) << "compiling fragment shader for " << k << " from " << filename;
      const auto source = Source::FromFile(filename);
      LOG_IF(FATAL, !source) << "failed to fetch fragment shader source for " << k << " from " << filename;
      const auto result = builder.AttachFragmentShader(source);
      LOG_IF(FATAL, !result.success()) << "failed to compile fragment shader: " << result;
    }

    Shader shader;
    auto result = builder.Build(&shader);
    LOG_IF(ERROR, !result.success()) << "failed to compile shader: " << result;
    return shader;
  }
}