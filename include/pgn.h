#ifndef OPENPGN_LIBRARY_H
#define OPENPGN_LIBRARY_H

#include <stdint.h>

enum pgnError {
  PGN_NOT_TAG,
  PGN_NOT_EXPECTED_EOF,
  PGN_NOT_ENOUGH_WHITESPACE,
  PGN_NO_VALUE,
  PGN_NOT_CLOSED,
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
  /// A zero-based row index. 0 means A, 7 means H.
  uint8_t row : 3;
  /// A zero-based column index. 0 means 1, 7 means 8.
  uint8_t col : 3;
  enum pgnPiece piece : 3;
  enum pgnColor color : 1;
} pgnMove;

#if __cplusplus
extern "C" {
#endif

enum pgnError pgnTags(const char **content, pgnTag buf[], uintptr_t* len);
enum pgnError pgnMoves(const char **content, pgnMove buf[], uintptr_t* len);

#if __cplusplus
}
#endif

#endif // OPENPGN_LIBRARY_H