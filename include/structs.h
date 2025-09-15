#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <stdbool.h>

#include "defines.h"

typedef struct
{
    uint8_t health;
} PhysicalChallenge;

typedef struct
{
    uint8_t firstNumber;
    uint8_t secondNumber;
} PuzzleChallenge;

typedef enum
{
    NONE,
    PHYSICAL,
    PUZZLE
} Challenge;

typedef enum
{
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;

typedef struct
{
    size_t roomNumber;
    char message[MAX_ROOM_MESSAGE_LENGTH];
    size_t connections[4]; // North, East, South, West
    Challenge challenge[MAX_CHALLENGES_PER_ROOM];
} Room;

typedef struct
{
    char name[PLAYER_NAME_LENGTH];
    Room currentRoom;
} Player;

typedef struct
{
    Player player;
    Room rooms[MAX_ROOMS];
    char response[MAX_RESPONSE_LENGTH];
} GameState;

#endif
