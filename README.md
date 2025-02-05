# OpenPGN

OpenPGN is open-source, light-weight PGN parser, written in C.

> [!NOTE]
> PGN is an abbreviation for Portable Game Notation, a standard plain text format for recording chess games
> For specification, See [Archive](https://ia802908.us.archive.org/26/items/pgn-standard-1994-03-12/PGN_standard_1994-03-12.txt)!

## Disclaimer

OpenPGN is fully compatible with standard PGN files, but also *<b>non-standard</b>* PGN files.

For example, Although a value of `Site` pair in Seven Tag Roaster should be in `City, Region COUNTRY` format,
Many platforms (especially, in online platforms) includes URL or doesn't use this format.

Like this, OpenPGN applies the rules very loosely to parse PGN.
Validating PGN in strict way is not yet implemented and also not planned.
(If you interested at it, please fork this repo and send me a pull request)
