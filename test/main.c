#include "sample.h"

#include <assert.h>
#include <pgn/pgn.h>
#include <stdio.h>
#include <stdlib.h>

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
  pgnMove moves[256];
  uintptr_t tagL;
  uintptr_t moveL;
  enum pgnError code;

  do {
    tagL = sizeof(tags) / sizeof(tags[0]);
    moveL = sizeof(moves) / sizeof(moves[0]);
    if (!((code = pgnTags(&cursor, tags, &tagL))) &&
        !((code = pgnMoves(&cursor, moves, &moveL)))) {
      continue;
    }

    fprintf(stderr, "error: failed with code %d\n", code);
    return code;
  } while (tagL > 0 && moveL > 0);

  return EXIT_SUCCESS;
}
