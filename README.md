# OpenPGN

OpenPGN is open-source, light-weight PGN parser, written in C99.

> [!NOTE]
> PGN is an abbreviation for Portable Game Notation, a standard plain text format for recording chess games
> For specification, See [Archive](https://ia802908.us.archive.org/26/items/pgn-standard-1994-03-12/PGN_standard_1994-03-12.txt)!

## Status

- [x] `pgnReadTags`
- [ ] `pgnReadMoves`

## Usage

```c
#include <pgn.h>

...

char* cursor = <file>;

intptr_t tagCount, moveCount;
pgnTag tags[256];
pgnMove moves[8192];

while ((tagCount  = pgnTags(&cursor, tags, 256))    > 0 &&
       (moveCount = pgnMoves(&cursor, moves, 8192)) > 0) {
    // do stuff...
}

if (tagCount < 0) {
    fprintf(stderr, "error: %ld", tagCount);
} else if (moveCount < 0) {
    fprintf(stderr, "error: %ld", moveCount);
}
```

You can use `pgnTags` and `pgnMoves` to read PGN file.
Umm... It's all of this library.

I have no plan to expand parser for rich features.
OpenPGN should be kept easy-to-use and simple.
If you don't think so, Mail me or Create discussion in this repo.

## Disclaimer

OpenPGN is fully compatible with standard PGN files, but also *<b>non-standard</b>* PGN files.

For example, Although a value of `Site` pair in Seven Tag Roaster should be in `City, Region COUNTRY` format,
Many platforms (especially, in online platforms) includes URL or doesn't use this format.

Like this, OpenPGN applies the rules very loosely to parse PGN.
Validating PGN in strict way is not yet implemented and also not planned.
(If you interested at it, please fork this repo and send me a pull request)
