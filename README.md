# OpenPGN

[![CTest](https://github.com/openpgn/openpgn/actions/workflows/ctest.yml/badge.svg)](https://github.com/openpgn/openpgn/actions/workflows/ctest.yml)
[![Doxygen](https://github.com/openpgn/openpgn/actions/workflows/doxygen.yml/badge.svg)](https://github.com/openpgn/openpgn/actions/workflows/doxygen.yml)

OpenPGN is single-pass, zero-copy PGN parser, written in C99.

> [!NOTE]
> PGN is an abbreviation for Portable Game Notation, a standard plain text format for recording chess games
> For specification,
> See [Archive](https://ia802908.us.archive.org/26/items/pgn-standard-1994-03-12/PGN_standard_1994-03-12.txt)!

## Missing Features

- Recursive Variation (See `18: Formal syntax` of archive document)

## Usage

```c
#include <pgn/pgn.h>

...
const char* cursor = ...;

pgnTag tags[256];
pgnMove moves[256];
uintptr_t tagL;
uintptr_t moveL;
enum pgnError code;

do {
  tagL = sizeof(tags) / sizeof(tags[0]);
  moveL = sizeof(moves) / sizeof(moves[0]);
  if ((code = pgnTags(&cursor, tags, &tagL)) ||
      (code = pgnMoves(&cursor, moves, &moveL))) {
    fprintf(stderr, "error: failed with code %d\n", code);
    return code;
  }
  
  // do stuff...

} while (tagL > 0 && moveL > 0);
```

You can use `pgnTags` and `pgnMoves` to read PGN file.
Umm... It's all of this library.

I have no plan to expand parser for rich features.
OpenPGN should be kept easy-to-use and simple.
If you don't think so, Open discussion or Issue feature request.

## Disclaimer

OpenPGN is fully compatible with standard PGN files, but also *<b>non-standard</b>* PGN files.

For example, Although a value of `Site` pair in Seven Tag Roaster should be in `City, Region COUNTRY` format,
Many platforms (especially, in online platforms) includes URL or doesn't use this format.

Like this, OpenPGN applies the rules very loosely to parse PGN.
Validating PGN in strict way is not yet implemented and also not planned.
(If you interested at it, please fork this repo and send me a pull request)
