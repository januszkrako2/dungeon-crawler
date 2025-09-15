#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "defines.h"
#include "structs.h"
#include "externs.h"
#include "text_utils.h"

void helpText(void)
{
    const char* helpText =
        "Type compass directions to move.\n"
        "Type 'attack' to attack.\n"
        "Type 'solve' followed by a number to solve puzzles.\n";
    
    printf("%s", helpText);
}

void ask(void)
{
    if (fgets(response, MAX_RESPONSE_LENGTH, stdin) == NULL)
    {
        perror("Error getting user input.");
        exit(1);
    }

    char* newlinePosition = strchr(response, '\n');

    if (newlinePosition == NULL)
    {
        while(getchar() != '\n');
    }
    else
    {
        *newlinePosition = '\0';
    }
}

void interpretInput(void)
{
    uint8_t reader = 0;
    uint8_t writer = 0;

    while (response[reader] != '\0')
    {
        if (!isspace(response[reader]))
        {
            response[writer] = response[reader];
            writer++;
        }

        reader++;
    }

    response[writer] = '\0';

    if (strncmp(response, "now", 3) == 0)
    {
        trimStart(response, 3);
    }

    if (strncmp(response, "go", 2) == 0)
    {
        trimStart(response, 2);
    }
}

void gameLogic(void)
{
    interpretInput();

    if (strncmp(response, "help", 4) == 0)
    {
        printf("\n");
        helpText();
        return;
    }
}
