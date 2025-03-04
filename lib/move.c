#include <ctype.h>
#include <pgn/pgn.h>
#include <pgn/util.h>
#include <string.h>

#define FILES "abcdefgh"
#define RANKS "12345678"
#define CAPTURE "x:"
#define CHECKS "#+"

#define IS(str) (memcmp(*content, str, strlen(str)) == 0)

int readPager(const char **content) {
  pgnStream stream;
  stream.content = content;

  skip(stream, WS);

  if (!skip(stream, NUM)) {
    return PGN_NO_SEQ_NUM;
  }
  const uintptr_t ellipsis = skip(stream, ".");
  if (ellipsis == 0 || ellipsis > 3) {
    return PGN_INV_SEQ_DELIMITER;
  }

  return PGN_SUCCESS;
}

int readMisc(const char **content, pgnMove *move) {
  pgnStream stream;
  stream.content = content;

  skip(stream, WS);

  // enpassant
  if (IS("e.p.")) {
    return PGN_SKIP;
  }
  // draw offer
  if (IS("=")) {
    return PGN_SKIP;
  }

  // forfeit
  if (IS("1/2-0")) {
    *content += 5;
    *move = PGN_BLACK_FORFEIT;
  }
  else if (IS("0-1/2")) {
    *content += 5;
    *move = PGN_WHITE_FORFEIT;
  }
  // win/defeat
  else if (IS("1-0")) {
    *content += 3;
    *move = PGN_WHITE_WIN;
  }
  else if (IS("1/2-1/2")) {
    *content += 7;
    *move = PGN_DRAW;
  }
  else if (IS("0-1")) {
    *content += 3;
    *move = PGN_BLACK_WIN;
  }
  else {
    return PGN_SKIP;
  }

  skip(stream, CHECKS);

  return PGN_SUCCESS;
}

int readMove(const char **content, pgnMove *move) {
  pgnStream stream;
  stream.content = content;

  move->fromFile = move->fromRank = move->toFile = move->toRank = 0;

  skip(stream, WS);

  // castling
  if (IS("0-0-0") || IS("O-O-O")) {
    *content += 5;
    *move = PGN_LONG_CASTLING;
  }
  else if (IS("0-0") || IS("O-O")) {
    *content += 3;
    *move = PGN_CASTLING;
  }
  if (move->fromFile /* if castling */) {
    skip(stream, CHECKS);
    return PGN_SUCCESS;
  }


  if (isupper(**content)) {
    switch (**content) {
#define PGN_PIECE(ch, p)                                                       \
  case ch:                                                                     \
    move->piece = p;                                                           \
    break;
      PGN_PIECE('K', PGN_KING);
      PGN_PIECE('Q', PGN_QUEEN);
      PGN_PIECE('B', PGN_BISHOP);
      PGN_PIECE('N', PGN_KNIGHT);
      PGN_PIECE('P', PGN_PAWN);
      PGN_PIECE('R', PGN_ROOK);
#undef PGN_PIECE
    default:
      return PGN_UNKNOWN_PIECE;
    }
    (*content)++;
  } else {
    move->piece = PGN_PAWN;
  }

  char code;

  if ((code = take(stream, FILES))) {
    move->toFile = code - 'a' + 1;
  }
  if ((code = take(stream, RANKS))) {
    move->toRank = code - '1' + 1;
  }

  take(stream, CAPTURE);

  if ((code = take(stream, FILES))) {
    move->fromFile = move->toFile;
    move->toFile = code - 'a' + 1;
  }
  if ((code = take(stream, RANKS))) {
    move->fromRank = move->toRank;
    move->toRank = code - '1' + 1;
  }

  skip(stream, CHECKS);

  if (!move->toFile) {
    return PGN_NO_FILE;
  }
  if (!move->toRank) {
    return PGN_NO_RANK;
  }

  return PGN_SUCCESS;
}

enum pgnError pgnMoves(const char **content, pgnMove buf[], uintptr_t *len) {
  uintptr_t i = 0;
  int code = 0;
  for (; i < *len; i++) {
    if ((code = readMisc(content, &buf[i])) != PGN_SKIP) {
      break;
    }

    const char *begin = *content;
    if (readPager(content) != PGN_SUCCESS) {
      *content = begin;
    }
    if ((code = readMove(content, &buf[i])))
      break;

    if (strncmp(*content, "\n\n", 2) == 0)
      break;
  }
  *len = i;
  return code;
}
