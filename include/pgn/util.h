#ifndef OPENPGN_UTIL_H
#define OPENPGN_UTIL_H

#include <stdint.h>

#define PGN_INTERNAL __attribute__((visibility("hidden")))

#define FAIL(c) ((c) <= 0)
#define EOF(c) ((c) < 0)
#define SUCCESS(c) ((c) > 0)

#define WS "\t\n\v\f\r "
#define ALNUM "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define NUM "0123456789"

#if __cplusplus
extern "C" {
#endif

PGN_INTERNAL char match(char ch, const char *list);
PGN_INTERNAL uintptr_t skip(const char **str, const char *list);
PGN_INTERNAL char accept(const char **str, const char *list);

#if __cplusplus
}
#endif

#endif // OPENPGN_UTIL_H
