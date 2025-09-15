/* 
TODO: replace all exit(1)'s with a function that freezes the program,
displays the error message and and only quits once the user presses enter.
This is for Windows compatibility -- when people double-click on the
executable, it opens a terminal window that instantly closes before
the user can see any error message.

TODO: Change integer types.

TODO: Remove defines, replace with compile-time constants.
*/ 

#include <stdint.h>
#include <stdio.h>

#include "defines.h"
#include "structs.h"
#include "externs.h"
#include "file_logic.h"
#include "game_logic.h"

Room rooms[MAX_ROOMS] = {0};
char response[MAX_RESPONSE_LENGTH] = {0};
Player player = {0};

int main(void)
{
    load();
    helpText();
    printf("Type 'help' at any time to get reminded of these instructions.\n");
    printf("\n");

    while (player.name[0] == '\0')
    {
        printf("What is your name? ");
        ask();

        for (size_t i = 0; i < MAX_RESPONSE_LENGTH; i++)
        {
            player.name[i] = response[i];
        }
    }

    // Main game loop
    while (1)
    {
        printf("What would you like to do? ");
        ask();
        gameLogic();
    }

    return 0;
}
