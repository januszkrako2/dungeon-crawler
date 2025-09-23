#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "constants.h"
#include "structs.h"
#include "mutables.h"

#include "text_utils.h"

void helpText(void)
{
    const char* helpText =
        "Type compass directions to move.\n"
        "Type 'attack' to attack.\n"
        "Type numbers to solve puzzles.\n";
    
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
    else if (strncmp(global.response, "solve", 5) == 0)
    {
        trimStart(global.response, 5);
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

    for (size_t i = 0; i < MAX_CHALLENGES_PER_ROOM; i++)
    {
        if (global.player.currentRoom.challenge[i] == PHYSICAL)
        {
            PhysicalChallenge delinquent = {0};
            delinquent.health = 2;

            printf("A delinquent appears! They look at you menacingly.\n");

            while (delinquent.health > 0)
            {
                printf("How do you respond? ");
                ask();

                if (strncmp(global.response, "attack", 6) == 0)
                {
                    delinquent.health--;

                    if (delinquent.health > 0)
                    {
                        printf(
                            "\nThe delinquent takes a hit.\n"
                        );
                    }
                    else
                    {
                        printf(
                            "\nThe delinquent falls and dies.\n"
                        );
                    }
                }
            }
        }
        else if (global.player.currentRoom.challenge[i] == PUZZLE)
        {
            srand(time(NULL));

            PuzzleChallenge puzzle;
            puzzle.firstNumber = rand() % 100 + 1;
            puzzle.secondNumber = rand() % 100 + 1;
            size_t answer = puzzle.firstNumber + puzzle.secondNumber;

            printf("There is a note on the floor. You pick it up.\n");
            printf("It says, '%zu + %zu'.\n",
                puzzle.firstNumber, puzzle.secondNumber);

            while (stringToSizeT(global.response) != answer)
            {
                printf("What could it possibly mean? ");
                ask();
            }

            printf("\nYou write '%s' on the note. You feel satisfied.\n",
                global.response);
        }

        // TODO: Fix all this logic, it doesn't work

        size_t j;

        for (j = 0; j < MAX_ROOMS; j++)
        {
            if (global.player.currentRoom.roomNumber ==
                global.rooms[j].roomNumber)
            {
                for (size_t k = 0; k < MAX_CHALLENGES_PER_ROOM; k++)
                {
                    if (global.rooms[j].challenge[k] != NONE)
                    {
                        global.rooms[j].challenge[k] = NONE;
                        break;
                    }
                }

                break;
            }
        }

        if (j == MAX_ROOMS)
        {
            perror("Cannot clear challenge from a room.");
            exit(1);
        }
    }
}
