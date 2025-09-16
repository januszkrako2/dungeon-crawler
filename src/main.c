/* 
TODO: replace all exit(1)'s with a function that freezes the program,
displays the error message and and only quits once the user presses enter.
This is for Windows compatibility -- when people double-click on the
executable, it opens a terminal window that instantly closes before
the user can see any error message.

TODO: Change integer types.

TODO: Refactor file logic.
*/ 

#include <stdint.h>
#include <stdio.h>

#include "constants.h"
#include "structs.h"
#include "mutables.h"

#include "text_utils.h"
#include "file_logic.h"
#include "game_logic.h"

GameState game = {0};

void printDebugInfo(void)
{
    printf("\n");
    printf("Player name: %s\n", game.player.name);
    printf("Last response: %s\n", game.response);
    printf("Introductory text: %s", game.introductoryText);
    printf("\n");
    printf("[ROOMS]\n");
    printf("\n");

    for (size_t i = 0; i < MAX_ROOMS; i++)
    {
        if (game.rooms[i].roomNumber == 0)
        {
            break;
        }

        printf("Room number: %lu\n", game.rooms[i].roomNumber);
        printf("Room message: %s\n", game.rooms[i].message);

        printf("North connection: %lu\n", game.rooms[i].connections[NORTH]);
        printf("East connection: %lu\n", game.rooms[i].connections[EAST]);
        printf("South connection: %lu\n", game.rooms[i].connections[SOUTH]);
        printf("West connection: %lu\n", game.rooms[i].connections[WEST]);

        printf("\n");

        for (size_t j = 0; j < MAX_CHALLENGES_PER_ROOM; j++)
        {
            printf("Challenge %zu: %u\n", j + 1, game.rooms[i].challenge[j]);
        }

        printf("\n");
    }
}

int main(void)
{
    load();
    helpText();
    printf("Type 'help' at any time to get reminded of these instructions.\n");
    printf("\n");

    while (game.player.name[0] == '\0')
    {
        printf("What is your name? ");
        ask();

        for (size_t i = 0; i < MAX_RESPONSE_LENGTH; i++)
        {
            game.player.name[i] = game.response[i];
        }
    }

    // Main game loop
    while (1)
    {
        printf("What would you like to do? ");
        ask();
        gameLogic();
        //printDebugInfo();
    }

    return 0;
}
