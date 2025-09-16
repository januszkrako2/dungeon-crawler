#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "constants.h"
#include "structs.h"
#include "mutables.h"

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
    if (fgets(game.response, MAX_RESPONSE_LENGTH, stdin) == NULL)
    {
        perror("Error getting user input.");
        exit(1);
    }

    char* newlinePosition = strchr(game.response, '\n');

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
    size_t read = 0;
    size_t write = 0;

    while (game.response[read] != '\0')
    {
        if (!isspace(game.response[read]))
        {
            game.response[write] = game.response[read];
            write++;
        }

        read++;
    }

    game.response[write] = '\0';

    if (strncmp(game.response, "now", 3) == 0)
    {
        trimStart(game.response, 3);
    }

    if (strncmp(game.response, "go", 2) == 0)
    {
        trimStart(game.response, 2);
    }
}

void gameLogic(void)
{
    interpretInput();

    if (strncmp(game.response, "help", 4) == 0)
    {
        printf("\n");
        helpText();
        return;
    }
}
