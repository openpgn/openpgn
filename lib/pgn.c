#include <ctype.h>
#include <pgn.h>
#include <stddef.h>

#define TAG 1

void SkipSpaces(char **content)
{
    while (isspace(**content)) (*content)++;
}

int SkipUntil(char **content, char until)
{
    char *cursor = *content;
    for (; *cursor; ++cursor)
        if (*cursor == until)
            break;
    *content = cursor;
    return *cursor != until;
}

void SkipAlnum(char **content)
{
    while (isalnum(**content)) (*content)++;
}

intptr_t pgnReadTags(char **content, pgnTag buffer[], uintptr_t bufferSize)
{
    int state = 0;
    int err = 0;
    int first = 1;
    intptr_t bufferCursor = 0;

    while (
        **content != '\0' &&
        !err &&
        bufferCursor < bufferSize &&
        !(**content == '\n' && *(*content + 1) == '\n'))
    {
        switch (state) // NOLINT(*-multiway-paths-covered)
        {
        case 0:
            SkipSpaces(content);

            if (**content == '[')
                state = TAG;
            else if (**content == ';') // line comment
                err = SkipUntil(content, '\n');
            else if (**content == '{') // block comment
                err = SkipUntil(content, '}');
            else if (first && **content == '%' && *(*content - 1) == '\n') // custom inst.
                err = SkipUntil(content, '\n');
            else
                err = PGN_UNEXPECTED_CHARACTER;

            ++(*content);
            continue;

        case TAG:
            SkipSpaces(content);
            if (!isalnum(**content))
            {
                err = PGN_UNEXPECTED_CHARACTER;
                continue;
            }
            buffer[bufferCursor].key = *content;
            SkipAlnum(content);
            if (!**content)
                continue;

            **content = '\0';
            ++(*content);

            SkipSpaces(content);
            if (**content != '"')
            {
                err = PGN_TAG_MISSING_VALUE;
                continue;
            }
            ++(*content);

            buffer[bufferCursor].value = *content;
            err = SkipUntil(content, '"');
            if (err != PGN_SUCCESS)
                continue;
            **content = '\0';
            ++(*content);

            SkipSpaces(content);
            if (**content != ']')
            {
                err = PGN_TAG_NOT_CLOSED;
                continue;
            }
            ++(*content);

            bufferCursor++;
            state = 0;
        }

        first = 0;
    }

    if (!err && **content == '\n' && *(*content + 1) == '\n')
        *content += 2;

EXIT:
    return err ? err : bufferCursor;
}
