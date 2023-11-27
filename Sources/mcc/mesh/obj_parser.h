#ifndef MCC_OBJ_PARSER_H
#define MCC_OBJ_PARSER_H

#include <cstdio>
#include "mcc/gfx.h"
#include "mcc/parser.h"
#include "mcc/platform.h"
#include "mcc/mesh/vertex.h"
#include "mcc/index_buffer.h"

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

  enum TokenKind {
    kUnknownToken,
    // keywords
#define DEFINE_TOKEN(Name, Keyword) k##Name##Token,
    FOR_EACH_OBJ_FILE_TOKEN_KEYWORD(DEFINE_TOKEN) 
#undef DEFINE_TOKEN
    // literals
#define DEFINE_TOKEN(Name) k##Name##Token,
    FOR_EACH_OBJ_FILE_TOKEN_LITERAL(DEFINE_TOKEN)
#undef DEFINE_TOKEN
    // symbols
#define DEFINE_TOKEN(Name, Symbol) k##Name##Token,
    FOR_EACH_OBJ_FILE_TOKEN_SYMBOL(DEFINE_TOKEN)
#undef DEFINE_TOKEN
    kEOFToken,
    kTotalNumberOfTokens,
  };

  static inline std::ostream&
  operator<<(std::ostream& stream, const TokenKind& rhs) {
    switch(rhs) {
      // keywords
#define DEFINE_TOSTRING(Name, Keyword) \
      case TokenKind::k##Name##Token: return stream << #Name;
      FOR_EACH_OBJ_FILE_TOKEN_KEYWORD(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      // literals
#define DEFINE_TOSTRING(Name) \
      case TokenKind::k##Name##Token: return stream << #Name;
      FOR_EACH_OBJ_FILE_TOKEN_LITERAL(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      // symbols
#define DEFINE_TOSTRING(Name, Symbol) \
      case TokenKind::k##Name##Token: return stream << #Name;
      FOR_EACH_OBJ_FILE_TOKEN_SYMBOL(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      case TokenKind::kEOFToken:
        return stream << "EOF";
      case TokenKind::kUnknownToken:
        return stream << "Unknown";
      default:
        return stream << "<unknown TokenKind: " << static_cast<uint64_t>(rhs) << ">";
    }
  }

  typedef TokenTemplate<TokenKind> Token;
  
  class ObjParser;
  typedef glm::u64vec3 FaceVertex;
  typedef bool (*OnFaceParsed)(const ObjParser* parser, uint64_t idx, FaceVertex* vertices, const uint64_t num_vertices);
  typedef bool (*OnVertexParsed)(const ObjParser* parser, const uint64_t idx, const glm::vec3& value);
  typedef bool (*OnNormalParsed)(const ObjParser* parser, const uint64_t idx, const glm::vec3& value);
  typedef bool (*OnUvParsed)(const ObjParser* parser, const uint64_t idx, const glm::vec2& value);

  static constexpr const uint64_t kDefaultObjParserBufferSize = 4096;
  static constexpr const uint64_t kDefaultObjParserTokenBufferSize = 1024;
  class ObjParser : public FileParserTemplate<TokenKind, kDefaultObjParserBufferSize, kDefaultObjParserTokenBufferSize> {
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
    Token peek_;
    glm::vec3 current_vec_;
    uint64_t num_vertices_;
    uint64_t num_normals_;
    uint64_t num_uvs_;
    uint64_t num_fvs_;
    uint64_t num_faces_;
    FaceVertex* face_vertices_;

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
      FileParserTemplate(file, data),
      config_(config),
      peek_(),
      current_vec_(),
      num_vertices_(0),
      num_normals_(0),
      num_uvs_(0),
      num_faces_(0),
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