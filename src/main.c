/* 
TODO: replace all exit(1)'s with a function that freezes the program,
displays the error message and and only quits once the user presses enter.
This is for Windows compatibility -- when people double-click on the
executable, it opens a terminal window that instantly closes before
the user can see any error message.

TODO: Change integer types.
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
    }

    return 0;
}
