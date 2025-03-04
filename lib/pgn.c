#include <pgn.h>
#include <string.h>

#define FAIL(c) ((c) <= 0)
#define EOF(c) ((c) < 0)
#define SUCCESS(c) ((c) > 0)

char match(const char ch, const char *list) {
  if (!ch)
    return -1;
  return (char)(strchr(list, ch) ? ch : 0);
}

uintptr_t skip(const char **str, const char *list) {
  uintptr_t size = 0;
  for (; **str; (*str)++, size++)
    if (FAIL(match(**str, list)))
      break;
  return size;
}

char accept(const char **str, const char *list) {
  const char r = match(**str, list);
  if (r >= 0)
    (*str)++;
  return r;
}

#define WS "\t\n\v\f\r "
#define ALNUM "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define NUM "0123456789"

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
