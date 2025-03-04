#include <pgn/util.h>
#include <string.h>

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
