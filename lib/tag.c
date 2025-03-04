#include <pgn/pgn.h>
#include <pgn/util.h>
#include <string.h>

int readTag(const char **content, pgnTag *tag) {

  char code = 0;
  char last = 0;

  skip(content, WS);

  if (FAIL(accept(content, "["))) {
    return PGN_NOT_TAG;
  }

  skip(content, WS);

  tag->key = *content;
  for (tag->keyLen = 0; !match(**content, WS) && SUCCESS(code = accept(content, ALNUM)); tag->keyLen++) {
    last = **content;
  }
  if (!last) // check EOF
    return PGN_NOT_EXPECTED_EOF;

  if (FAIL(match(last, WS))) {
    return PGN_NOT_ENOUGH_WHITESPACE;
  }

  skip(content, WS);

  if (FAIL(accept(content, "\""))) {
    return PGN_NO_VALUE;
  }

  tag->value = *content;
  for (tag->valueLen = 0; FAIL(code = accept(content, "\"")) && !EOF(code); tag->valueLen++) {
  }
  if (EOF(code))
    return PGN_NOT_EXPECTED_EOF;

  skip(content, WS);

  if (FAIL(accept(content, "]"))) {
    return PGN_NOT_CLOSED;
  }

  return 0;
}

enum pgnError pgnTags(const char **content, pgnTag buf[], uintptr_t *len) {
  uintptr_t i = 0;
  int code = 0;
  for (; i < *len && !((code = readTag(content, &buf[i]))); i++)
    if (strncmp(*content, "\n\n", 2) == 0)
      break;
  *len = i;
  return code;
}
