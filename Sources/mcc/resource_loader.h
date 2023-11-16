#ifndef MCC_RESOURCE_H
#error "Please #include <mcc/resource.h> instead."
#endif //MCC_RESOURCE_H

#ifndef MCC_RESOURCE_LOADER_H
#define MCC_RESOURCE_LOADER_H

#include <optional>
#include "mcc/json.h"
#include "mcc/resource_reference.h"

namespace mcc {
  namespace resource {
    template<typename R>
    class Loader {
    protected:
      Tag tag_;

      explicit Loader(const Tag& tag):
        tag_(tag) {
      }
    public:
      virtual ~Loader() = default;
      virtual Reference<R> Load() = 0;

      Tag tag() const {
        return tag_;
      }
    };

    template<typename R>
    class FileLoader : public Loader<R> {
    protected:
      std::string filename_;
      FILE* file_;

      explicit FileLoader(const Tag& tag,
                          const std::string& filename):
        Loader<R>(tag),
        filename_(filename),
        file_(NULL) {
        if((file_ = fopen(filename_.c_str(), "rb")) == NULL)
          DLOG(ERROR) << "failed to open " << filename_ << ": " << ferror(file_);
      }

      inline void Close() {
        if(!IsOpen())
          return;
        fclose(file_);
        file_ = NULL;
      }
    public:
      ~FileLoader() override {
        Close();
      }

      bool IsOpen() const {
        return file_ != NULL;
      }
    };

    template<typename R>
    class JsonFileLoader : public FileLoader<R> {
    protected:
      json::Document doc_;
    
      explicit JsonFileLoader(const Tag& tag,
                              const std::string& filename):
        FileLoader<R>(tag, filename) {
        if(!ParseJson(FileLoader<R>::file_, doc_)) {
          DLOG(ERROR) << "failed to parse json document from: " << FileLoader<R>::filename_;
          FileLoader<R>::Close();
        }
      }

      inline std::optional<std::string> GetDocumentString(const char* name) const {
        if(!doc_.HasMember(name))
          return std::nullopt;
        const auto& value = doc_[name];
        if(!value.IsString()) {
          return std::nullopt;
        }
        return std::optional<std::string>{
          std::string(value.GetString(), value.GetStringLength()),
        };
      }

      inline std::optional<std::string> GetDocumentType() const {
        return GetDocumentString("type");
      }
    public:
      ~JsonFileLoader() override = default;
    };
  }
}

#endif //MCC_RESOURCE_LOADER_H