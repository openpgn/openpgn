#ifndef OPENPGN_UTIL_H
#define OPENPGN_UTIL_H

#include <stdint.h>
#include <stdbool.h>

#define PGN_INTERNAL __attribute__((visibility("hidden")))

#define WS "\t\n\v\f\r "
#define ALNUM "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define NUM "0123456789"

#if __cplusplus
extern "C" {
#endif

typedef struct __pgnStream {
  const char **content;
} pgnStream;

PGN_INTERNAL bool is(pgnStream stream, const char *list);
PGN_INTERNAL bool eof(pgnStream stream);
PGN_INTERNAL char take(pgnStream stream, const char *list);
PGN_INTERNAL uintptr_t skip(pgnStream stream, const char *list);
PGN_INTERNAL uintptr_t until(pgnStream stream, const char *list);
PGN_INTERNAL const char *cursor(pgnStream stream);

#if __cplusplus
}
#endif

#endif // OPENPGN_UTIL_H
