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
  V(UseMaterial, usemtl)

#define FOR_EACH_OBJ_FILE_TOKEN_SYMBOL(V) \
  V(LParen, '(')                          \
  V(RParen, ')')                          \
  V(ForwardSlash, '/')                    \
  V(Space, ' ')

#define FOR_EACH_OBJ_FILE_TOKEN_LITERAL(V) \
  V(Integer)                               \
  V(Float)                

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

    const char* text;

    Token():
      kind(Token::kUnknown),
      pos(0, 0),
      text(nullptr) {
    }
    Token(const Kind k, const Position p):
      kind(k),
      pos(p) {  
    }
    Token(const Kind k,
          const uint64_t r,
          const uint64_t c):
      kind(k),
      pos(r, c),
      text(nullptr) { 
    }
    Token(const Kind k,
          const uint64_t r,
          const uint64_t c,
          const char* t,
          const uint64_t l):
      kind(k),
      pos(r, c),
      text(strndup(t, l)) { 
    }
    Token(const Kind k,
          const uint64_t r,
          const uint64_t c,
          const char s):
      kind(k),
      pos(r, c),
      text(strndup(&s, 1)) {  //TODO: this is dumb af
    }
    Token(const Token& rhs):
      kind(rhs.kind),
      pos(rhs.pos),
      text(rhs.text ? strdup(rhs.text) : nullptr) {
    }
    ~Token() {
      if(text)
        free((void*) text);
    }

    bool valid() const {
      return kind != Token::kUnknown;
    }

    bool invalid() const {
      return kind == Token::kUnknown;
    }

    bool has_text() const {
      return text != nullptr;
    }

    void operator=(const Token& rhs) {
      kind = rhs.kind;
      pos = rhs.pos;
      if(text)
        free((void*) text);
      text = nullptr;
      if(rhs.text)
        text = strdup(rhs.text);
    }

    friend std::ostream& operator<<(std::ostream& stream, const Token& rhs) {
      stream << "Token(";
      stream << "kind=" << rhs.kind << ", ";
      stream << "pos=" << rhs.pos;
      if(rhs.text) {
        stream << ", ";
        stream << "text=" << rhs.text;
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

  class ObjParser {
  public:
    struct Config {
      OnFaceParsed on_face_parsed;
      OnVertexParsed on_vertex_parsed;
      OnNormalParsed on_normal_parsed;
      OnUvParsed on_uv_parsed;
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
    
    glm::vec3 current_vertex_;
    uint64_t num_vertices_;
    
    glm::vec3 current_normal_;
    uint64_t num_normals_;
    
    glm::vec2 current_uv_;
    uint64_t num_uvs_;

    FaceVertex current_fv_;
    uint64_t num_faces_;

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
    
    inline char PeekChar() {
      int c = getc(file_);
      ungetc(c, file_);
      return (char) c;
    }

    inline char NextChar() {
      const auto c = (char)getc(file_);
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
      while(IsWhitespace(next = NextChar()));
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
  public:
    ObjParser(const Config& config, FILE* file, void* data):
      config_(config),
      file_(file),
      data_(data),
      peek_(),
      row_(1),
      column_(1),
      current_vertex_(),
      num_vertices_(0),
      current_normal_(),
      num_normals_(0),
      current_uv_(),
      num_uvs_(0),
      current_fv_(),
      num_faces_(0) {
    }
    explicit ObjParser(FILE* file, void* data = nullptr):
      ObjParser(Config{}, file, data) {  
    }
    ~ObjParser() = default;

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