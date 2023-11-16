#ifndef MCC_JSON_H
#define MCC_JSON_H

#include <cstdio>
#include <glog/logging.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

namespace mcc {
  namespace json=rapidjson;

  bool ParseJson(FILE* file, json::Document& doc);

  static inline bool
  ParseJson(const std::string& filename, json::Document& doc) {
    DLOG(INFO) << "parsing json from " << filename;
    auto file = fopen(filename.c_str(), "rb");
    if(!file)
      return false;
    return ParseJson(file, doc);
  }
}

#endif //MCC_JSON_H