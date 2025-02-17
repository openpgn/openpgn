#include "pgn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[]) {
  FILE *f = NULL;
  long size = 0;
  const char *filename = NULL;
  char *buffer = NULL;
  char *cursor = NULL;
  pgnTag tags[256];
  uintptr_t len = sizeof tags / sizeof tags[0];
  enum pgnError code;
  char strBuf[256];

  if (argc != 2) {
    fprintf(stderr,
            "error: invalid invocation\n"
            "usage: %s [filename]\n",
            argv[0]);
    goto EXIT;
  }

  filename = argv[1];
  f = fopen(filename, "r");
  if (f == NULL) {
    perror(filename);
    goto EXIT;
  }

  fseek(f, 0, SEEK_END);
  size = ftell(f);
  fseek(f, 0, SEEK_SET);

  cursor = buffer = malloc(size + 1);
  if (buffer == NULL) {
    perror("malloc");
    goto EXIT;
  }

  fread(buffer, 1, size, f);
  fclose(f);

  buffer[size] = '\0';


  if ((code = pgnReadTags(&cursor, tags, &len))) {
    fprintf(stderr, "error: pgnReadTags failed with code %d\n", code);
    goto EXIT;
  }

  for (int i = 0; i < len; i++) {
    strncpy(strBuf, tags[i].key, tags[i].keyLen);
    strBuf[tags[i].keyLen] = 0;
    printf("%s = ", strBuf);

    strncpy(strBuf, tags[i].value, tags[i].valueLen);
    strBuf[tags[i].valueLen] = 0;
    printf("%s\n", strBuf);
  }

EXIT:
  if (buffer)
    free(buffer);
  return EXIT_SUCCESS;
}
