#include "pgn.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[])
{
    if (argc != 2)
    {
        fprintf(
            stderr,
            "error: invalid invocation\n"
            "usage: %s [filename]\n",
            argv[0]);
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];
    FILE* f = fopen(filename, "r");
    if (f == NULL)
    {
        perror(filename);
        return EXIT_FAILURE;
    }

    fseek(f, 0, SEEK_END);
    const long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    if (buffer == NULL)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    fread(buffer, 1, size, f);
    fclose(f);

    buffer[size] = '\0';

    pgnTag tags[256];

    char* cursor = buffer;
    intptr_t code = pgnReadTags(&cursor, tags, 256);
    if (code < 0)
    {
        fprintf(stderr, "error: pgnReadTags failed with code %ld\n", code);
        goto EXIT;
    }

    for (int i = 0; i < code; i++)
    {
        printf("%s = %s\n", tags[i].key, tags[i].value);
    }


EXIT:
    free(buffer);
    return EXIT_SUCCESS;
}
