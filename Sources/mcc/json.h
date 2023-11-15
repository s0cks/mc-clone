#ifndef MCC_JSON_H
#define MCC_JSON_H

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

namespace mcc {
  namespace json=rapidjson;

  bool ParseJson(const std::string& filename, json::Document& doc);
}

#endif //MCC_JSON_H