#ifndef STRUCT_H
#define STRUCT_H

#include <stdbool.h>

#include "constant.h"

struct physical_challenge {
	int32_t health;
};

struct puzzle_challenge {
	size_t first;
	size_t second;
};

enum challenge {
	NONE,
	PHYSICAL,
	PUZZLE
};

enum direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

struct room {
	size_t room_number;
	size_t connections[4]; // North, East, South, West
	enum challenge challenge[MAX_CHALLENGES_PER_ROOM];
	char message[MAX_ROOM_MESSAGE_LENGTH];
};

struct connection {
	const char* text;
	size_t size;
};

struct player {
	char name[PLAYER_NAME_LENGTH];
	struct room room;
};

struct game {
	struct player player;
	struct room rooms[MAX_ROOMS];
	char response[MAX_RESPONSE_LENGTH];
	char introductory_text[INTRO_TEXT_MAX_LENGTH];
};

#endif
