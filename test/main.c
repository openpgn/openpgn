#include "sample.h"

#include <assert.h>
#include <pgn.h>
#include <stdio.h>
#include <stdlib.h>

#define PGN_DUMP_TO_STDOUT 0

int run(const char *cursor);

int main() {
  assert(run(&__sample) == 0);
  assert(run(&__sample_fail_1) == 1);
  assert(run(&__sample_fail_2_0) == 2);
  assert(run(&__sample_fail_2_1) == 2);
  assert(run(&__sample_fail_3) == 3);
  assert(run(&__sample_fail_4) == 4);
  assert(run(&__sample_fail_5) == 5);

  return 0;
}

int run(const char *cursor) {
  pgnTag tags[256];
  uintptr_t len = sizeof(tags) / sizeof(tags[0]);
  enum pgnError code;

  if ((code = pgnTags(&cursor, tags, &len))) {
    fprintf(stderr, "error: pgnReadTags failed with code %d\n", code);
    return code;
  }

#if PGN_DUMP_TO_STDOUT
  char strBuf[256];
  for (int i = 0; i < len; i++) {
    strncpy(strBuf, tags[i].key, tags[i].keyLen);
    strBuf[tags[i].keyLen] = 0;
    printf("%s = ", strBuf);

    strncpy(strBuf, tags[i].value, tags[i].valueLen);
    strBuf[tags[i].valueLen] = 0;
    printf("%s\n", strBuf);
  }
#endif

  return EXIT_SUCCESS;
}
