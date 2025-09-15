#include <string.h>
#include <stdint.h>

void trimStart(char* text, uint8_t characters)
{
    uint8_t stringLength = strlen(text);
    for (uint8_t i = 0; i < stringLength; i++)
    {
        if ((i + characters) <= stringLength)
        {
            text[i] = text[i + characters];
        }
        else
        {
            text[i] = '\0';
        }
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