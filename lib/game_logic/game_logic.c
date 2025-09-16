#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

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
    if (fgets(global.response, MAX_RESPONSE_LENGTH, stdin) == NULL)
    {
        perror("Error getting user input.");
        exit(1);
    }

    char* newlinePosition = strchr(global.response, '\n');

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

    while (global.response[read] != '\0')
    {
        if (!isspace(global.response[read]))
        {
            global.response[write] = global.response[read];
            write++;
        }

        read++;
    }

    global.response[write] = '\0';

    if (strncmp(global.response, "go", 2) == 0)
    {
        trimStart(global.response, 2);
    }
    else if (strncmp(global.response, "now", 3) == 0)
    {
        trimStart(global.response, 3);
    }
}

void gameLogic(void)
{
    interpretInput();

    size_t roomNumberToFind = 0;
    bool moved = false;

    if (strncmp(global.response, "help", 4) == 0)
    {
        printf("\n");
        helpText();
        return;
    }
    else if (strncmp(global.response, "north", 5) == 0)
    {
        moved = true;
        roomNumberToFind = global.player.currentRoom.connections[NORTH];
    }
    else if (strncmp(global.response, "east", 4) == 0)
    {
        moved = true;
        roomNumberToFind = global.player.currentRoom.connections[EAST];
    }
    else if (strncmp(global.response, "south", 5) == 0)
    {
        moved = true;
        roomNumberToFind = global.player.currentRoom.connections[SOUTH];
    }
    else if (strncmp(global.response, "west", 4) == 0)
    {
        moved = true;
        roomNumberToFind = global.player.currentRoom.connections[WEST];
    }

    if (moved)
    {
        if (roomNumberToFind == 0)
        {
            printf("\nYou hit a wall. Ouch!\n");
            return;
        }

        size_t i;

        for (i = 0; i < MAX_ROOMS; i++)
        {
            if (global.rooms[i].roomNumber == roomNumberToFind)
            {
                global.player.currentRoom = global.rooms[i];
                break;
            }
        }

        if (i == MAX_ROOMS)
        {
            perror("Couldn't find room.");
            exit(1);
        }
        
        printf("\n%s", global.player.currentRoom.message);
    }
}
