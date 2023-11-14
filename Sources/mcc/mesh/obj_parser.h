#ifndef MCC_OBJ_PARSER_H
#define MCC_OBJ_PARSER_H

#include <cstdio>
#include "mcc/gfx.h"
#include "mcc/platform.h"

#include "mcc/mesh/index.h"
#include "mcc/mesh/vertex.h"

namespace mcc {
#define FOR_EACH_OBJ_FILE_TOKEN_KEYWORD(V) \
  V(Vertex, v)                              \
  V(Normal, vn)                             \
  V(Texcoord, vt)                           \
  V(Face, f)                                \
  V(FaceObject, fo)                         \
  V(Object, o)                              \
  V(Group, g)                               \
  V(MaterialLib, mtllib)                    \
  V(UseMaterial, usemtl)                    \
  V(Line, l)                                \
  V(SmoothShading, s)                       \
  V(ParameterVertex, vp)

#define FOR_EACH_OBJ_FILE_TOKEN_SYMBOL(V) \
  V(LParen, '(')                          \
  V(RParen, ')')                          \
  V(ForwardSlash, '/')                    \
  V(Space, ' ')

#define FOR_EACH_OBJ_FILE_TOKEN_LITERAL(V) \
  V(Integer)                               \
  V(Float)                                 \
  V(Identifier)           

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

  struct Token {
  public:
    static constexpr const uint64_t kMaxLength = 1024;
    enum Kind {
      kUnknown,
      // keywords
#define DEFINE_TOKEN(Name, Keyword) k##Name,
      FOR_EACH_OBJ_FILE_TOKEN_KEYWORD(DEFINE_TOKEN) 
#undef DEFINE_TOKEN
      // literals
#define DEFINE_TOKEN(Name) k##Name,
      FOR_EACH_OBJ_FILE_TOKEN_LITERAL(DEFINE_TOKEN)
#undef DEFINE_TOKEN
      // symbols
#define DEFINE_TOKEN(Name, Symbol) k##Name,
      FOR_EACH_OBJ_FILE_TOKEN_SYMBOL(DEFINE_TOKEN)
#undef DEFINE_TOKEN
      kEOF,
      kTotalNumberOfTokens,
    };

    friend std::ostream& operator<<(std::ostream& stream, const Kind& rhs) {
      switch(rhs) {
        // keywords
#define DEFINE_TOSTRING(Name, Keyword) \
        case Kind::k##Name: return stream << #Name;
        FOR_EACH_OBJ_FILE_TOKEN_KEYWORD(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
        // literals
#define DEFINE_TOSTRING(Name) \
        case Kind::k##Name: return stream << #Name;
        FOR_EACH_OBJ_FILE_TOKEN_LITERAL(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      // symbols
#define DEFINE_TOSTRING(Name, Symbol) \
        case Kind::k##Name: return stream << #Name;
      FOR_EACH_OBJ_FILE_TOKEN_SYMBOL(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
        case Kind::kEOF:
          return stream << "EOF";
        case Kind::kUnknown:
          return stream << "Unknown";
        default:
          return stream << "<unknown Token::Kind: " << static_cast<uint64_t>(rhs) << ">";
      }
    }
  public:
    Kind kind;
    Position pos;
    uint8_t* data;
    uint64_t length;

    Token():
      kind(Token::kUnknown),
      pos(0, 0),
      data(nullptr),
      length(0) {
    }
    Token(const Kind k, const Position p):
      kind(k),
      pos(p),
      data(nullptr),
      length(0) {
    }
    Token(const Kind k,
          const uint64_t r,
          const uint64_t c):
      kind(k),
      pos(r, c),
      data(nullptr),
      length(0) { 
    }
    Token(const Kind k,
          const uint64_t r,
          const uint64_t c,
          uint8_t* t,
          const uint64_t l):
      kind(k),
      pos(r, c),
      data(t),
      length(l) {
    }
    Token(const Token& rhs):
      kind(rhs.kind),
      pos(rhs.pos),
      data(rhs.data),
      length(rhs.length) {
    }
    ~Token() {}

    bool valid() const {
      return kind != Token::kUnknown;
    }

    bool invalid() const {
      return kind == Token::kUnknown;
    }

    uint64_t as_u64() const {
      return static_cast<uint64_t>(atoll((const char*) data));
    }

    float as_float() const {
      return static_cast<float>(atof((const char*) data));
    }

    void operator=(const Token& rhs) {
      kind = rhs.kind;
      pos = rhs.pos;
      data = rhs.data;
      length = rhs.length;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Token& rhs) {
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
  
  class ObjParser;
  typedef glm::u64vec3 FaceVertex;
  typedef bool (*OnFaceParsed)(const ObjParser* parser, uint64_t idx, FaceVertex* vertices, const uint64_t num_vertices);
  typedef bool (*OnVertexParsed)(const ObjParser* parser, const uint64_t idx, const glm::vec3& value);
  typedef bool (*OnNormalParsed)(const ObjParser* parser, const uint64_t idx, const glm::vec3& value);
  typedef bool (*OnUvParsed)(const ObjParser* parser, const uint64_t idx, const glm::vec2& value);

  static constexpr const uint64_t kDefaultObjParserBufferSize = 4096;
  static constexpr const uint64_t kDefaultObjParserTokenBufferSize = 1024;
  class ObjParser {
  public:
    struct Config {
      OnFaceParsed on_face_parsed;
      OnVertexParsed on_vertex_parsed;
      OnNormalParsed on_normal_parsed;
      OnUvParsed on_uv_parsed;
      uint64_t num_face_vertices;

      Config():
        on_face_parsed(nullptr),
        on_vertex_parsed(nullptr),
        on_normal_parsed(nullptr),
        on_uv_parsed(nullptr),
        num_face_vertices(3) {
      }
    };

    struct Stats {
      uint64_t vertices;
      uint64_t normals;
      uint64_t uvs;
      uint64_t faces;

      Stats() = default;
      Stats(const Stats& rhs) = default;
      ~Stats() = default;
      Stats& operator=(const Stats& rhs) = default;
    };
  private:
    Config config_;
    void* data_;
    FILE* file_;
    Token peek_;
    uint64_t row_;
    uint64_t column_;
    uint64_t nread_;
    uint64_t rpos_;
    uint8_t buffer_[kDefaultObjParserBufferSize];
    uint8_t token_[Token::kMaxLength];
    uint64_t token_length_;
    glm::vec3 current_vec_;
    uint64_t num_vertices_;
    uint64_t num_normals_;
    uint64_t num_uvs_;
    uint64_t num_fvs_;
    uint64_t num_faces_;
    FaceVertex* face_vertices_;

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

    inline bool ReadNextChunk() {
      memset(buffer_, 0, sizeof(buffer_));
      nread_ = fread(buffer_, sizeof(uint8_t), kDefaultObjParserBufferSize, file_);
      rpos_ = 0;
      return nread_ > 0;
    }
    
    inline char PeekChar() {
      if(rpos_ >= nread_) {
        if(!ReadNextChunk())
          return EOF;
      }
      return (char)buffer_[rpos_];
    }

    inline char NextChar() {
      if((rpos_ + 1) > nread_) {
        if(!ReadNextChunk())
          return EOF;
      }

      const auto c = (char)buffer_[rpos_];
      rpos_ += 1;
      switch(c) {
        case '\n':
          row_ += 1;
          column_ = 1;
          return c;
        default:
          column_ += 1;
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

    inline bool HasPeek() const {
      return !peek_.invalid();
    }

    Token PeekToken(const bool skip_whitespace = true);
    Token NextToken(const bool skip_whitespace = true);
    bool ParseFace();
    bool ParseVertex(glm::vec3& result);
    bool ParseNormal(glm::vec3& result);
    bool ParseTexcoord(glm::vec2& result);
    bool ParseFaceVertex(FaceVertex& result);
    bool ParseFloat(float* result);
    bool ParseLong(uint64_t* result);
    bool ParseUseMaterial();
    bool ParseMaterialLib();
  public:
    ObjParser(const Config& config, FILE* file, void* data):
      config_(config),
      file_(file),
      data_(data),
      peek_(),
      row_(1),
      column_(1),
      current_vec_(),
      num_vertices_(0),
      num_normals_(0),
      num_uvs_(0),
      num_faces_(0),
      rpos_(0),
      nread_(0),
      buffer_(),
      token_(),
      token_length_(0),
      face_vertices_(nullptr) {
      if(config.num_face_vertices > 0) {
        face_vertices_ = (FaceVertex*)malloc(sizeof(FaceVertex) * config.num_face_vertices);
        FaceVertex fv;
        memset(face_vertices_, 0, sizeof(FaceVertex) * config.num_face_vertices);
        for(auto idx = 0; idx < config.num_face_vertices; idx++) {
          memcpy(&face_vertices_[idx], &fv, sizeof(FaceVertex));
        }
      }
    }
    explicit ObjParser(FILE* file, void* data = nullptr):
      ObjParser(Config{}, file, data) {  
    }
    ~ObjParser() {
      if(face_vertices_ && config_.num_face_vertices > 0)
        free(face_vertices_);
    }

    void* data() const {
      return data_;
    }

    bool ParseFile();

    Stats stats() const {
      return Stats {
        .vertices = num_vertices_,
        .normals = num_normals_,
        .uvs = num_uvs_,
        .faces = num_faces_,
      };
    }
  };
}

#endif //MCC_OBJ_PARSER_H