#ifndef OPENPGN_PGN_H
#define OPENPGN_PGN_H

#include <stdint.h>

enum pgnError {
  PGN_SUCCESS = 0,

  PGN_NO_BEGIN_BRACKET,
  PGN_NO_KEY,
  PGN_NO_DELIMITER,
  PGN_NO_BEGIN_QUOTE,
  PGN_NO_END_QUOTE,
  PGN_NO_END_BRACKET
};

enum pgnColor : uint8_t { PGN_BLACK = 0, PGN_WHITE = 1 };

enum pgnPiece : uint8_t {
  PGN_PAWN = 0,
  PGN_KNIGHT = 1,
  PGN_BISHOP = 2,
  PGN_ROOK = 3,
  PGN_QUEEN = 4,
  PGN_KING = 5
};

typedef struct __pgnTag {
  uintptr_t keyLen;
  uintptr_t valueLen;
  const char *key;
  const char *value;
} pgnTag;

typedef struct __pgnMove {
  /// (optional) A one-based row index. 1 means A, 8 means H. 0 means none.
  uint8_t fromFile : 4;
  /// (optional) A one-based column index. 0 means none.
  uint8_t fromRank : 4;
  /// A one-based row index. 1 means A, 8 means H.
  uint8_t toFile : 4;
  /// A one-based column index.
  uint8_t toRank : 4;
  enum pgnPiece piece : 3;
} pgnMove;

#if __cplusplus
extern "C" {
#endif

__attribute__((visibility("default")))
enum pgnError pgnTags(const char **content, pgnTag buf[], uintptr_t* len);
__attribute__((visibility("default")))
enum pgnError pgnMoves(const char **content, pgnMove buf[], uintptr_t* len);

#if __cplusplus
}
#endif

#endif // OPENPGN_PGN_H