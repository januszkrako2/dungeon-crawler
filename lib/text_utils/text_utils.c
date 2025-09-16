#include <string.h>
#include <stdint.h>

void trimStart(char* text, size_t characters)
{
    size_t i = 0;

    while (text[i + characters] != '\0')
    {
        text[i] = text[i + characters];
        i++;
    }

    for (size_t j = 0; j < characters; j++)
    {
        text[i + j] = '\0';
    }
}

size_t stringToSizeT(char* text)
{
    size_t value = 0;
    size_t i = 0;

    while (text[i] != '\0')
    {
        if (text[i] >= '0' && text[i] <= '9')
        {
            value = (value * 10) + text[i] - '0';
        }

        i++;
    }

    return value;
}
