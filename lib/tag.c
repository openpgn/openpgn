#include <pgn/pgn.h>
#include <pgn/util.h>
#include <string.h>

int readTag(const char **content, pgnTag *tag) {

  struct __pgnStream stream;
  stream.content = content;

  skip(&stream, WS);

  if (!take(&stream, "["))
    return PGN_NO_BEGIN_BRACKET;

  if (!is(stream, ALNUM))
    return PGN_NO_KEY;

  tag->key = cursor(stream);
  tag->keyLen = skip(&stream, ALNUM);
  if (!skip(&stream, WS))
    return PGN_NO_DELIMITER;

  if (!take(&stream, "\""))
    return PGN_NO_BEGIN_QUOTE;

  tag->value = cursor(stream);
  tag->valueLen = until(&stream, "\"\n");
  if (!take(&stream, "\""))
    return PGN_NO_END_QUOTE;

  if (!take(&stream, "]"))
    return PGN_NO_END_BRACKET;

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
