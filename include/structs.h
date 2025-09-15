#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <stdbool.h>

#include "defines.h"

typedef struct PhysicalChallenge
{
	uint8_t health;
} PhysicalChallenge;

typedef struct PuzzleChallenge
{
	uint8_t firstNumber;
	uint8_t secondNumber;
} PuzzleChallenge;

typedef enum Challenge
{
	NONE,
	PHYSICAL,
	PUZZLE
} Challenge;

typedef enum Direction
{
	NORTH,
	EAST,
	SOUTH,
	WEST
} Direction;

typedef struct Room
{
	size_t roomNumber;
	char message[MAX_ROOM_MESSAGE_LENGTH];
	size_t connections[4]; // North, East, South, West
	Challenge challenge[MAX_CHALLENGES_PER_ROOM];
} Room;

typedef struct Player
{
	char name[PLAYER_NAME_LENGTH];
	Room currentRoom;
} Player;

#endif
