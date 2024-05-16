#ifndef MCC_PARSER_H
#define MCC_PARSER_H

#include <cstdint>
#include <ostream>

namespace mcc {
  struct Position {
    uint64_t row;
    uint64_t column;

    Position() = default;
    Position(const uint64_t r, const uint64_t c):
      row(r),
      column(c) {
    }
    Position(const Position& rhs) = default;
    ~Position() = default;

    Position& operator=(const Position& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const Position& rhs) {
      stream << "(" << rhs.row << ", " << rhs.column << ")";
      return stream;
    }
  };

  template<typename Kind>
  struct TokenTemplate {
  public:
    static constexpr const uint64_t kMaxLength = 1024;
  public:
    Kind kind;
    Position pos;
    uint8_t* data;
    uint64_t length;

    TokenTemplate():
      kind(Kind::kUnknownToken),
      pos(0, 0),
      data(nullptr),
      length(0) {
    }
    TokenTemplate(const Kind k, const Position& p):
      kind(k),
      pos(p),
      data(nullptr),
      length(0) {
    }
    TokenTemplate(const Kind k,
                  const uint64_t r,
                  const uint64_t c):
      TokenTemplate(k, Position(r, c)) {
    }
    TokenTemplate(const Kind k,
                  const Position& p,
                  uint8_t* t,
                  const uint64_t l):
      kind(k),
      pos(p),
      data(t),
      length(l) {
    }
    TokenTemplate(const Kind k,
                  const uint64_t r,
                  const uint64_t c,
                  uint8_t* t,
                  const uint64_t l):
      TokenTemplate(k, Position(r, c), t, l) {
    }
    TokenTemplate(const TokenTemplate<Kind>& rhs):
      kind(rhs.kind),
      pos(rhs.pos),
      data(rhs.data),
      length(rhs.length) {
    }
    ~TokenTemplate() {}

    bool valid() const {
      return kind != Kind::kUnknownToken;
    }

    bool invalid() const {
      return kind == Kind::kUnknownToken;
    }

    uint64_t as_u64() const {
      return static_cast<uint64_t>(atoll((const char*) data));
    }

    float as_float() const {
      return static_cast<float>(atof((const char*) data));
    }

    void operator=(const TokenTemplate<Kind>& rhs) {
      kind = rhs.kind;
      pos = rhs.pos;
      data = rhs.data;
      length = rhs.length;
    }

    friend std::ostream& operator<<(std::ostream& stream, const TokenTemplate<Kind>& rhs) {
      stream << "Token(";
      stream << "kind=" << rhs.kind << ", ";
      stream << "pos=" << rhs.pos;
      if(rhs.length > 0) {
        stream << ", ";
        stream << "text=" << std::string((const char*) rhs.data, rhs.length);
      }
      stream << ")";
      return stream;
    }
  };

  template<const uint64_t ParserBufferSize,
           const uint64_t TokenBufferSize>
  class ParserTemplate {
  protected:
    void* data_;
    Position pos_;
    uint8_t buffer_[ParserBufferSize];
    uint64_t buffer_len_;
    uint64_t rpos_;
    uint8_t token_[TokenBufferSize];
    uint64_t token_len_;

    explicit ParserTemplate(void* data):
      data_(data),
      pos_(1, 1),
      buffer_(),
      buffer_len_(0),
      rpos_(0),
      token_(),
      token_len_(0) {
    }
    ParserTemplate(void* data, const uint8_t* bytes, const uint64_t nbytes):
      data_(data),
      pos_(1, 1),
      buffer_(),
      buffer_len_(0),
      rpos_(0),
      token_(),
      token_len_(0) {
      CopyBufferFrom(bytes, nbytes);
    }
    ParserTemplate(void* data, const std::string& buffer):
      ParserTemplate(data, (const uint8_t*) &buffer[0], static_cast<uint64_t>(buffer.length())) {
    }

    inline void CopyBufferFrom(const uint8_t* data, const uint64_t nbytes) {
      if(data == nullptr || nbytes <= 0)
        return;
      const auto len = std::min(nbytes, ParserBufferSize);
      memcpy(&buffer_[0], data, len);
      buffer_len_ = len;
    }

    inline uint64_t row() const {
      return pos_.row;
    }

    inline uint64_t column() const {
      return pos_.column;
    }

    inline Position token_start() const {
      return Position(row(), column() - token_len_);
    }

    static inline bool IsWhitespace(const char c) {
      switch(c) {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
          return true;
        default:
          return false;
      }
    }

    static inline bool IsDigit(const char c) {
      return (c >= '0') && (c <= '9');
    }

    virtual bool ReadNextChunk() {
      return false;
    }

    inline char PeekChar() {
      if(rpos_ >= buffer_len_) {
        if(!ReadNextChunk())
          return EOF;
      }
      return (char)buffer_[rpos_];
    }

    inline char NextChar() {
      if((rpos_ + 1) > buffer_len_) {
        if(!ReadNextChunk())
          return EOF;
      }

      const auto c = (char)buffer_[rpos_];
      rpos_ += 1;
      switch(c) {
        case '\n':
          pos_.row += 1;
          pos_.column = 1;
          return c;
        default:
          pos_.column += 1;
          return c;
      }
    }

    inline char NextRealChar() {
      char next;
      do {
        next = NextChar();
      } while(IsWhitespace(next));
      return next;
    }

    void set_data(const void* data) {
      data_ = data;
    }
  public:
    virtual ~ParserTemplate() = default;

    void* data() const {
      return data_;
    }
  };

  template<const uint64_t ParserBufferSize,
           const uint64_t TokenBufferSize>
  class FileParserTemplate : public ParserTemplate<ParserBufferSize, TokenBufferSize> {
  private:
    typedef ParserTemplate<ParserBufferSize, TokenBufferSize> Parent;
  protected:
    FILE* file_;

    FileParserTemplate(FILE* file, void* data):
      ParserTemplate<ParserBufferSize, TokenBufferSize>(data),
      file_(file) {
    }

    bool ReadNextChunk() override {
      memset(Parent::buffer_, 0, sizeof(Parent::buffer_));
      Parent::buffer_len_ = fread(Parent::buffer_, sizeof(uint8_t), ParserBufferSize, file_);
      Parent::rpos_ = 0;
      return Parent::buffer_len_ > 0;
    }
  public:
    ~FileParserTemplate() override = default;

    FILE* file() const {
      return file_;
    }
  };
}

#endif //MCC_PARSER_H