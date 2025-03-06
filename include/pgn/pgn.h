#ifndef OPENPGN_PGN_H
#define OPENPGN_PGN_H

#include <stdint.h>

enum pgnError {
  PGN_SUCCESS = 0,

  /* pgnTags */

  /// There is no begin bracket for a tag
  PGN_NO_BEGIN_BRACKET,
  /// There is no key for a tag
  PGN_NO_KEY,
  /// There is no delimiter for a tag
  PGN_NO_DELIMITER,
  /// There is no begin quote for a tag
  PGN_NO_BEGIN_QUOTE,
  /// There is no end quote for a tag
  PGN_NO_END_QUOTE,
  /// There is no end bracket for a tag
  PGN_NO_END_BRACKET,

  /* pgnMoves */

  /// There is no sequence number for a sequence notation
  PGN_NO_SEQ_NUM,
  /// There is invalid delimiter for a sequence notation
  PGN_INV_SEQ_DELIMITER,

  /// There is unknown piece at a move
  PGN_UNKNOWN_PIECE,
  /// There is no file at a move
  PGN_NO_FILE,
  /// There is no rank at a move
  PGN_NO_RANK,

  /// There is invalid NAG
  PGN_INV_NAG,
  /// There is no end brace for a comment
  PGN_NO_END_BRACE,

  /* internals */
  PGN_EOF,
  PGN_SKIP,
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

/// A class that represents a tag pair.
typedef struct __pgnTag {
  uintptr_t keyLen;
  uintptr_t valueLen;
  const char *key;
  const char *value;
} pgnTag;

/// A class that represents a movement of piece(s).
typedef struct __pgnMove {
  /// (optional) A one-based row index. 1 means A, 8 means H. 0 means none.
  /// If `fromFile` and `fromRank` are -1, this move represents special state
  /// such as castling, winning and forfeit.
  uint8_t fromFile : 4;
  /// (optional) A one-based column index. 0 means none.
  /// If `fromFile` and `fromRank` are -1, this move represents special state
  /// such as castling, winning and forfeit.
  uint8_t fromRank : 4;
  /// A one-based row index. 1 means A, 8 means H.
  uint8_t toFile : 4;
  /// A one-based column index.
  uint8_t toRank : 4;
  enum pgnPiece piece : 3;
} pgnMove;

/// King-side castling (short castling)
static const pgnMove PGN_CASTLING = {-1, -1};
/// Queen-side castling (long castling)
static const pgnMove PGN_LONG_CASTLING = {-1, -1, -1};
/// A movement that represents Black wins the game.
static const pgnMove PGN_BLACK_WIN = {-1, -1, -1, 1};
/// A movement that represents White wins the game.
static const pgnMove PGN_WHITE_WIN = {-1, -1, -1, 2};
/// A movement that represents Black violates a rule (forfeit).
static const pgnMove PGN_BLACK_FORFEIT = {-1, -1, -1, 3};
/// A movement that represents White violates a rule (forfeit).
static const pgnMove PGN_WHITE_FORFEIT = {-1, -1, -1, 4};
/// A movement that represents stalemates
static const pgnMove PGN_DRAW = {-1, -1, -1, 5};

#if __cplusplus
extern "C" {
#endif

/**
 * Parses tag-section of pgn-game.
 * See `18. Formal Syntax` of
 * https://ia802908.us.archive.org/26/items/pgn-standard-1994-03-12/PGN_standard_1994-03-12.txt
 *
 * @param content A pointer to cursor (document)
 * @param buf A buffer to store tag-pairs
 * @param len A pointer to length of buffer. The number of tags will be stored
 * in this address.
 * @return An error code with `pgnError`
 */
__attribute__((visibility("default"))) enum pgnError
pgnTags(const char **content, pgnTag buf[], uintptr_t *len);

/**
 * Parses movetext-section of pgn-game.
 * See `18. Formal Syntax` of
 * https://ia802908.us.archive.org/26/items/pgn-standard-1994-03-12/PGN_standard_1994-03-12.txt
 *
 * @param content A pointer to cursor (document)
 * @param buf A buffer to store movements
 * @param len A pointer to length of buffer. The number of movements will be
 * stored in this address.
 * @return An error code with `pgnError`
 */
__attribute__((visibility("default"))) enum pgnError
pgnMoves(const char **content, pgnMove buf[], uintptr_t *len);

#if __cplusplus
}
#endif

#endif // OPENPGN_PGN_H