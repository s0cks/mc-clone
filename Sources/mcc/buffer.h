#ifndef MCC_BUFFER_H
#define MCC_BUFFER_H

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <glog/logging.h>

#include "mcc/common.h"

namespace mcc {
  class Buffer {
    DEFINE_NON_COPYABLE_TYPE(Buffer);
  public:
    static constexpr const uint64_t kDefaultBufferSize = 4096;
  protected:
    uint8_t* data_;
    uint64_t capacity_;
    uint64_t wpos_;
    uint64_t rpos_;
  public:
    explicit Buffer(const uint64_t init_cap):
      data_(nullptr),
      capacity_(0),
      wpos_(0),
      rpos_(0) {
        if(init_cap > 0) {
          const auto cap = init_cap * sizeof(uint8_t);
          const auto data = reinterpret_cast<uint8_t*>(malloc(sizeof(uint8_t) * cap));
          if(!data) {
            DLOG(ERROR) << "cannot allocate buffer of size " << cap;
            return;
          }
          VLOG(3) << "allocated new buffer of size " << cap;
          data_ = data;
          capacity_ = cap;
          memset(data_, 0, sizeof(uint8_t) * capacity_);
        }
    }
    Buffer(const uint8_t* data, const uint64_t capacity):
      data_(nullptr),
      capacity_(),
      wpos_(capacity),
      rpos_(0) {
      CopyFrom(data, capacity);
    }
    ~Buffer() {
      if(data_) {
        VLOG(3) << "freeing buffer of size " << capacity_;
        free(data_);
      }
    }

    inline void
    Resize(const uint64_t new_size) {
      if(new_size < 0 || new_size <= capacity_)
        return;
      const auto new_cap = RoundUpPow2(new_size);
      const auto data = (uint8_t*)realloc(data_, new_cap);
      LOG_IF(FATAL, !data) << "failed to reallocate Buffer to " << new_cap << " bytes.";
      data_ = data;
      capacity_ = new_cap;
      DLOG(INFO) << "resized Buffer to " << new_cap << " bytes.";
    }

    inline void
    CopyFrom(const uint8_t* src, const uint64_t nbytes) {
      Resize(nbytes);
      MCC_ASSERT(capacity_ >= nbytes);
      memcpy(data_, src, sizeof(uint8_t) * nbytes);
    }
  public:
    uint8_t* data() const {
      return data_;
    }

    uint64_t capacity() const {
      return capacity_;
    }
    
    uint64_t write_pos() const {
      return wpos_;
    }

    void set_write_pos(const uint64_t pos) {
      wpos_ = pos;
    }

    uint64_t read_pos() const {
      return rpos_;
    }

    void set_read_pos(const uint64_t pos) {
      rpos_ = pos;
    }

    bool empty() const {
      return capacity() == 0 || write_pos() == 0;
    }

    template<typename T>
    bool Insert(const T value, const uint64_t pos) {
      const uint64_t tsize = sizeof(T);
      if((pos + tsize) > capacity())
        return false;
      memcpy(&data()[pos], &value, tsize);
      wpos_ = pos + tsize;
      return true;
    }

    template<typename T>
    bool Append(const T value) {
      return Insert<T>(value, wpos_);
    }

    template<typename T>
    T Read(const uint64_t pos) {
      const uint64_t tsize = sizeof(T);
      if((pos + tsize) > capacity()) {
        DLOG(ERROR) << "cannot insert " << tsize << " bytes into " << (*this) << " @" << pos;
        return false;
      }
      const T data = *(reinterpret_cast<T*>(data() + pos));
      rpos_ = pos + tsize;
      return data;
    }

    template<typename T>
    T Read() {
      return Read<T>(rpos_);
    }

    bool WriteTo(FILE* file) const {
      if(!file)
        return false;
      if(fwrite(data(), sizeof(uint8_t), write_pos(), file) != 0) {
        DLOG(ERROR) << "couldn't write " << (*this) << " to file: " << strerror(errno);
        return false;
      }
      return true;
    }

    bool ReadFrom(FILE* file, const uint64_t pos, const uint64_t nbytes) {
      if((pos + nbytes) > capacity_) {
        DLOG(ERROR) << "cannot read " << nbytes << " from file, buffer is full.";
        return false;
      }
      if(fread(&data_[pos], sizeof(uint8_t), nbytes, file) != nbytes) {
        DLOG(ERROR) << "couldn't read " << capacity_ << " bytes from file: " << strerror(errno);
        return false;
      }
      VLOG(3) << "read " << nbytes << " from file.";
      wpos_ = pos + nbytes;
      return true;
    }

    bool ReadFrom(FILE* file, const uint64_t nbytes) {
      return ReadFrom(file, wpos_, nbytes);
    }

    bool ReadFrom(FILE* file) {
      return ReadFrom(file, capacity_);
    }

    bool Put(const uint8_t* data, const uint64_t pos, const uint64_t nbytes) {
      if((pos + nbytes) > capacity_) {
        VLOG(3) << "cannot put " << nbytes << " in " << (*this) << " at " << pos;
        return false;
      }
      memcpy(&data_[pos], data, nbytes);
      wpos_ = pos + nbytes;
      return true;
    }

    bool Put(const uint8_t* data, const uint64_t nbytes) {
      return Put(data, wpos_, nbytes);
    }

    friend std::ostream& operator<<(std::ostream& stream, const Buffer& rhs) {
      stream << "Buffer(";
      stream << "data=" << std::hex << rhs.data() << ", ";
      stream << "capacity=" << rhs.capacity();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_BUFFER_H