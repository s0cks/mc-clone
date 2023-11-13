#include "mcc/mesh/obj_parser.h"

namespace mcc {
#define UNEXPECTED_TOKEN(Level, Next) \
  LOG(Level) << "unexpected token: " << (Next);

#define EXPECT_NEXT(Name) ({                                    \
  const auto next = PeekToken(false);                           \
  switch(next.kind) {                                           \
    case Token::k##Name:                                        \
      NextToken(false);                                         \
      break;                                                    \
    default:                                                    \
      UNEXPECTED_TOKEN(ERROR, next);                            \
      return false;                                             \
  }                                                             \
})

  bool ObjParser::ParseFile() {
    do {
      auto next = NextToken();
      switch(next.kind) {
        case Token::kVertex: {
          if(!ParseVertex(current_vec_)) {
            LOG(ERROR) << "failed to parse vertex.";
            return false;
          }
          num_vertices_++;
          if(config_.on_vertex_parsed)
            config_.on_vertex_parsed(this, num_vertices_, current_vec_);
          break;
        }
        case Token::kTexcoord: {
          if(!ParseTexcoord((glm::vec2&) current_vec_)) {
            LOG(ERROR) << "failed to parse texcoord.";
            return false;
          }
          num_uvs_++;
          if(config_.on_uv_parsed)
            config_.on_uv_parsed(this, num_uvs_, current_vec_);
          break;
        }
        case Token::kNormal: {
          if(!ParseNormal(current_vec_)) {
            LOG(ERROR) << "failed to parse normal.";
            return false;
          }
          num_normals_++;
          if(config_.on_normal_parsed)
            config_.on_normal_parsed(this, num_normals_, current_vec_);
          break;
        }
        case Token::kFace: {
          if(!ParseFace()) {
            LOG(ERROR) << "failed to parse face.";
            return false;
          }
          break;
        }
        case Token::kEOF:
          return true;
        default:
          UNEXPECTED_TOKEN(ERROR, next);
          return false;
      }
    } while(true);
    return true;
  }

  bool ObjParser::ParseLong(uint64_t* result) {
    const auto next = NextToken();
    switch(next.kind) {
      case Token::kInteger:
        MCC_ASSERT(next.length > 0);
        (*result) = static_cast<uint64_t>(atoll((const char*) next.data));
        return true;
      default:
        UNEXPECTED_TOKEN(ERROR, next);
        return false;
    }
  }

  bool ObjParser::ParseFloat(float* result) {
    const auto next = NextToken();
    switch(next.kind) {
      case Token::kFloat:
        MCC_ASSERT(next.length > 0);
        (*result) = static_cast<float>(atof((const char*) next.data));
        return true;
      case Token::kInteger:
        MCC_ASSERT(next.length > 0);
        (*result) = static_cast<float>(atoll((const char*) next.data));
        return true;
      default:
        UNEXPECTED_TOKEN(ERROR, next);
        return false;
    }
  }

  bool ObjParser::ParseVertex(glm::vec3& result) {
    if(!ParseFloat(&result[0]))
      return false;
    EXPECT_NEXT(Space);
    if(!ParseFloat(&result[1]))
      return false;
    EXPECT_NEXT(Space);
    if(!ParseFloat(&result[2]))
      return false;
    return true;
  }

  bool ObjParser::ParseTexcoord(glm::vec2& result) {
    if(!ParseFloat(&result[0]))
      return false;
    if(!ParseFloat(&result[1]))
      return false;
    return true;
  }

  bool ObjParser::ParseNormal(glm::vec3& result) {
    if(!ParseFloat(&result[0]))
      return false;
    if(!ParseFloat(&result[1]))
      return false;
    if(!ParseFloat(&result[2]))
      return false;
    return true;
  }

  bool ObjParser::ParseFaceVertex(FaceVertex& result) {
    if(!ParseLong(&result[0]))
      return false;
    auto next = PeekToken(false);
    switch(next.kind) {
      case Token::kSpace:
      case Token::kFace:
        break;
      case Token::kForwardSlash:
        next = NextToken(false);
        next = PeekToken(false);
        switch(next.kind) {
          case Token::kForwardSlash:
            next = NextToken(false);
            // v1//vn1
            if(!ParseLong(&result[2]))
              return false;
            return true;
          case Token::kInteger:
            // v1/vt1
            if(!ParseLong(&result[1]))
              return false;
            next = PeekToken(false);
            switch(next.kind) {
              case Token::kForwardSlash:
                next = NextToken(false);
                // v1/vt1/vn1
                if(!ParseLong(&result[2]))
                  return false;
                return true;
              case Token::kSpace:
              case Token::kFace:
                return true;
              default:
                UNEXPECTED_TOKEN(ERROR, next);
                return false;
            }
            return true;
          default:
            UNEXPECTED_TOKEN(ERROR, next);
            return false;
        }
        return true;
      default:
        UNEXPECTED_TOKEN(ERROR, next);
        return false;
    }
    return false;
  }

  bool ObjParser::ParseFace() {
    num_fvs_ = 0;
    do {
      auto next = PeekToken();
      switch(next.kind) {
        case Token::kInteger: {
          if(!ParseFaceVertex(face_vertices_[num_fvs_]))
            return false;
          num_fvs_++;
          break;
        }
        case Token::kFace:
        case Token::kEOF:
          goto finish_face;
        default:
          UNEXPECTED_TOKEN(ERROR, next);
          return false;
      }
    } while(true);
  finish_face:
    num_faces_++;
    if(config_.on_face_parsed)
      config_.on_face_parsed(this, num_faces_, face_vertices_, num_fvs_);
    return true;
  }

  Token ObjParser::PeekToken(const bool skip_whitespace) {
    if(HasPeek())
      return peek_;
    peek_ = NextToken(skip_whitespace);
    return peek_;
  }

  Token ObjParser::NextToken(const bool skip_whitespace) {
    if(HasPeek()) {
      const auto next = peek_;
      peek_ = Token();
      return next;
    }

    char next = skip_whitespace ? NextRealChar() : NextChar();
    // symbol check
    switch(next) {
      case EOF: return Token(Token::kEOF, row_, column_);
#define DEFINE_NEXT_SYMBOL(Name, Symbol) \
      case Symbol: return Token(Token::k##Name, row_, column_, &buffer_[rpos_ - 1], 1);
      FOR_EACH_OBJ_FILE_TOKEN_SYMBOL(DEFINE_NEXT_SYMBOL)
#undef DEFINE_NEXT_SYMBOL
      default: break;
    }

    // keyword check
    switch(next) {
      case 'v': {
        char peek = PeekChar();
        switch(peek) {
          case 'n':
            NextChar();
            return Token(Token::kNormal, row_, column_);
          case 't':
            NextChar();
            return Token(Token::kTexcoord, row_, column_);
          case ' ':
            return Token(Token::kVertex, row_, column_);
          default:
            return Token(Token::kUnknown, row_, column_, &buffer_[rpos_ - 1], 1);
        }
        break;
      }
      case 'f':
        return Token(Token::kFace, row_, column_);
      default:
        break;
    }

    if(IsDigit(next) || next == '-') {
      token_start_ = rpos_ - 1;
      bool whole = true;
      bool negative = next == '-';
      while(IsDigit(next = PeekChar()) || next == '.' || next == 'e' || next == '-') {
        if(next == '.')
          whole = false;
        next = NextChar();
      }

      token_end_ = rpos_ - 1;
      token_length_ = (token_end_ - token_start_) + 1;
      if(whole) {
        return Token(Token::kInteger, row_, column_, &buffer_[token_start_], token_length_);
      } else {
        return Token(Token::kFloat, row_, column_, &buffer_[token_start_], token_length_);
      }
    }

    return Token(Token::kUnknown, row_, column_);
  }
}