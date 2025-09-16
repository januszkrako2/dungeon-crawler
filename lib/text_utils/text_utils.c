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
    size_t convertedValue = 0;
    size_t stringLength = strlen(text);

    for (size_t i = 0; i < stringLength; i++)
    {
        if (text[i] >= '0' && text[i] <= '9')
        {
            convertedValue = (convertedValue * 10) + text[i] - 48;
        }
    }
    
    return convertedValue;
}
