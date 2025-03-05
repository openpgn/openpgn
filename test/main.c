#include "sample.h"

#include <assert.h>
#include <pgn/pgn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PGN_DUMP_TO_STDOUT 0

int run(const char *cursor);

int main() {
  assert(run(&__sample) == 0);
  assert(run(&__sample_no_begin_bracket) == PGN_NO_BEGIN_BRACKET);
  assert(run(&__sample_no_key) == PGN_NO_KEY);
  assert(run(&__sample_no_delimiter) == PGN_NO_DELIMITER);
  assert(run(&__sample_no_begin_quote) == PGN_NO_BEGIN_QUOTE);
  assert(run(&__sample_no_end_quote) == PGN_NO_END_QUOTE);
  assert(run(&__sample_no_end_bracket) == PGN_NO_END_BRACKET);

  // PGN_NO_SEQ_NUM is ignored internally to handle seq number easily
  // assert(run(&__sample_no_seq_num) == PGN_NO_SEQ_NUM);
  assert(run(&__sample_inv_seq_delimiter) == PGN_INV_SEQ_DELIMITER);

  assert(run(&__sample_unknown_piece) == PGN_UNKNOWN_PIECE);
  assert(run(&__sample_no_file) == PGN_NO_FILE);
  assert(run(&__sample_no_rank) == PGN_NO_RANK);

  return 0;
}

int run(const char *cursor) {
  pgnTag tags[256];
  uintptr_t tagL = sizeof(tags) / sizeof(tags[0]);
  pgnMove moves[256];
  uintptr_t moveL = sizeof(moves) / sizeof(moves[0]);
  enum pgnError code;

  if ((code = pgnTags(&cursor, tags, &tagL))) {
    fprintf(stderr, "error: pgnTags failed with code %d\n", code);
    return code;
  }
  if ((code = pgnMoves(&cursor, moves, &moveL))) {
    fprintf(stderr, "error: pgnMoves failed with code %d\n", code);
    return code;
  }

  printf("T:%lu M:%lu\n", tagL, moveL);

#if PGN_DUMP_TO_STDOUT
  char strBuf[256];
  for (int i = 0; i < tagL; i++) {
    strncpy(strBuf, tags[i].key, tags[i].keyLen);
    strBuf[tags[i].keyLen] = 0;
    printf("%s = ", strBuf);

    strncpy(strBuf, tags[i].value, tags[i].valueLen);
    strBuf[tags[i].valueLen] = 0;
    printf("%s\n", strBuf);
  }

  for (int i = 0; i < moveL; i++) {
    printf(
      "%c %d %c %d\n",
      moves[i].fromFile + 'a' - 1, moves[i].fromRank,
      moves[i].toFile + 'a' - 1, moves[i].toRank);
  }
#endif

  return EXIT_SUCCESS;
}
