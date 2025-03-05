#include <assert.h>
#include <pgn/util.h>
#include <string.h>

bool is(const pgnStream stream, const char *list) {
  assert(list != NULL);
  assert(stream.content != NULL && *stream.content != NULL);

  return strchr(list, **stream.content);
}

bool eof(const pgnStream stream) {
  assert(stream.content != NULL && *stream.content != NULL);

  return **stream.content == 0;
}

char take(const pgnStream* stream, const char *list) {
  if (!is(*stream, list))
    return 0;

  const char r = **stream->content;
  (*stream->content)++;
  return r;
}

uintptr_t skip(const pgnStream* stream, const char *list) {
  uintptr_t size = 0;
  for (; is(*stream, list); size++) {
    (*stream->content)++;
  }

  return size;
}

uintptr_t until(const pgnStream* stream, const char *list) {
  uintptr_t size = 0;
  for (; !eof(*stream) && !is(*stream, list); size++) {
    (*stream->content)++;
  }

  return size;
}

const char *cursor(pgnStream stream) {
  return *stream.content;
}
