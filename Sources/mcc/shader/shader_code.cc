#include "mcc/shader/shader_code.h"

#include <sstream>
#include <fmt/format.h>
#include "mcc/flags.h"
#include "mcc/sha256.h"
#include "mcc/common.h"

namespace mcc::shader {
  int ShaderCode::Compare(const RawShaderCode bytes, const uword num_bytes) const {
    if(GetLength() < num_bytes)
      return -1;
    else if(!bytes || GetLength() > num_bytes)
      return +1;
    return memcmp(raw_ptr(), bytes, num_bytes);
  }

  std::string ShaderCode::ToString() const {
    using namespace units::data;
    std::stringstream ss;
    ss << "ShaderCode(";
    ss << "type=" << GetType() << ", ";
    ss << "length=" << GetLength() << ", ";
    ss << "total_size=" << byte_t(GetSize()) << ", ";
    ss << "code=" << GetSHA256().ToHexString();
    ss << ")";
    return ss.str();
  }

  void* ShaderCode::operator new(const size_t size, const uword length) { // TODO: round up pow2
    using namespace units::data;
    const auto total_size = CalculateAllocateSize(length);
    const auto ptr = malloc(total_size);
    memset(ptr, 0, sizeof(ptr));
    LOG_IF(FATAL, !ptr) << "failed to allocate ShaderCode of " << byte_t(total_size) << " size.";
    DLOG(INFO) << "allocated " << byte_t(total_size) << " for ShaderCode.";
    return ptr;
  }

  ShaderCode* ShaderCode::New(const ShaderType type, const RawShaderCode bytes, const uword num_bytes) {
    if(!bytes || num_bytes == 0)
      return nullptr;
    const auto code = new(num_bytes) ShaderCode(type, num_bytes);
    code->CopyFrom(bytes, num_bytes);
    return code;
  }

  ShaderCode* ShaderCode::Copy(const ShaderCode* rhs) {
    if(!rhs)
      return nullptr;
    const auto code = new(rhs->GetLength()) ShaderCode(rhs->GetType(), rhs->GetLength());
    code->CopyFrom(rhs);
    return code;
  }

  ShaderCode* ShaderCode::FromFile(const ShaderType type, const uri::Uri& uri) {
    if(uri.HasScheme("shader")) {
      const auto new_uri = uri::Uri("file", fmt::format("{0:s}/shaders/{1:s}", FLAGS_resources, uri.path));
      return FromFile(type, new_uri);
    } else if(!uri.HasScheme("file")) {
      LOG(ERROR) << "cannot load ShaderCode from: " << uri;
      return nullptr;
    }

    MCC_ASSERT(uri.HasScheme("file"));
    const auto file = uri.OpenFileForReading();
    if(!file) {
      LOG(ERROR) << "failed to open: " << uri;
      return nullptr;
    }
    const auto total_size = GetFilesize(file);
    const auto code = new(total_size)ShaderCode(type, total_size);
    const auto nread = fread(code->raw_ptr(), sizeof(uint8_t), total_size, file);
    if(nread != total_size) {
      using namespace units::data;
      LOG(ERROR) << "failed to read ShaderCode, read " << byte_t(nread) << "/" << byte_t(total_size) << " from " << uri << ": " << strerror(errno);
      delete code;
      return nullptr;
    }
    return code;
  }
  
  ShaderCode* ShaderCode::FromFile(const uri::Uri& uri) {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  uint256 ShaderCode::GetSHA256() const {
    return sha256::Of(GetData(), GetLength());
  }
}