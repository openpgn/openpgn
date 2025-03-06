#include "sample.h"

#include <assert.h>
#include <pgn/pgn.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PRINT_ERROR 1
#define BENCHMARK 1
#define BENCHMARK_ITERATIONS 1

int run(const char *cursor);

struct timespec diff(const struct timespec start, const struct timespec end)
{
  struct timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp;
}

#if BENCHMARK
int benchmark(const char* name, const char *cursor) {
  struct timespec start, end;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

  printf("=== BEGIN %s\n", name);
  fflush(stdout);

  for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
    const int code = run(cursor);
    if (code) {
      return code;
    }
  }

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

  const struct timespec d = diff(start, end);
  printf("=== END %s : %ldms\n", name, (d.tv_sec * 1000 + d.tv_nsec / 1000000) / BENCHMARK_ITERATIONS);
  fflush(stdout);

  return 0;
}
#endif

int main() {

#if BENCHMARK
  int code;
  if ((code = benchmark("caro-kann", &__caro_kann))) return code;
  if ((code = benchmark("anglo-slav", &__anglo_slav))) return code;
#endif

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
  pgnMove moves[512];
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

#if PRINT_ERROR
    fprintf(stderr, "error: failed with code %d\n", code);
#endif
    return code;
  } while (tagL > 0 && moveL > 0);

  return EXIT_SUCCESS;
}
