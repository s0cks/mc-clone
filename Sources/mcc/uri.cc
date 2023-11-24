#include <unordered_set>

#include "mcc/uri.h"

namespace mcc::uri {
  struct ProtocolHasher {
    std::size_t operator()(const ProtocolPtr& k) const {
      using std::size_t;
      using std::hash;
      using std::string;
      return hash<string>()(k->GetName());
    }
  };

  struct ProtocolEquals {
    bool operator()(const ProtocolPtr& lhs, const ProtocolPtr& rhs) const {
      if(!lhs && !rhs)
        return true;
      if((lhs && !rhs) || (!lhs && rhs))
        return false;
      return lhs->GetName() == rhs->GetName();
    }
  };

  static std::unordered_set<ProtocolPtr, ProtocolHasher, ProtocolEquals> protocols_;

  static inline std::optional<ProtocolPtr>
  ParseUriProtocol(const basic_uri& data) {
    const auto first_colon = data.find_first_of(':');
    if(first_colon == std::string::npos)
      return std::nullopt;
    const auto protoc = data.substr(0, first_colon);
    for(const auto& protocol : protocols_) {
      if(protocol->Matches(protoc))
        return protocol;
    }
    DLOG(ERROR) << "cannot find protocol '" << protoc << "'";
    return std::nullopt;
  }

  static inline void
  ParseProtocol(const basic_uri& uri, std::string* result) {
    for(const auto& protocol : protocols_) {
      DLOG(INFO) << "checking if " << uri << " matches: " << protocol->GetName();
      if(protocol->Matches(uri)) {
        (*result) = protocol->GetName();
        return;
      }
    }
    DLOG(ERROR) << "cannot find protocol for " << uri;
    (*result) = "file";
  }

  static inline void
  ParseLocation(const basic_uri& uri, std::string* result) {
    const auto first_colon = uri.find_first_of(':');
    if(first_colon == std::string::npos) {
      (*result) = uri;
      return;
    }
    (*result) = uri.substr(first_colon + 3);
  }

  Uri::Uri(const basic_uri& uri):
    protocol(),
    location() {
    ParseProtocol(uri, &protocol);
    ParseLocation(uri, &location);
  }

  bool RegisterProtocol(const ProtocolPtr& protocol) {
    if(!protocol) {
      DLOG(ERROR) << "cannot register a null protocol.";
      return false;
    }
    const auto pos = protocols_.insert(protocol);
    DLOG_IF(WARNING, !pos.second) << "failed to register the '" << protocol->GetName() << "' protocol";
    return pos.second;
  }
}