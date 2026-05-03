#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "constant.h"
#include "struct.h"
#include "mutable.h"

#include "utility.h"

void help_text(void) {
	const char* help_text =
	"Type compass directions to move.\n"
	"Type 'attack' to attack.\n"
	"Type numbers to solve puzzles.\n";

	printf("%s", help_text);
}

static void interpret_input(void) {
	size_t read = 0;
	size_t write = 0;
	while (game.response[read] != '\0') {
		if (isspace(game.response[read])) {
			read++;
			continue;
		}

		if (game.response[read] >= 'A' && game.response[read] <= 'Z') {
			game.response[read] += 32;
		}

		game.response[write] = game.response[read];
		write++;
		read++;
	}
	game.response[write] = '\0';

	if (strncmp(game.response, "go", 2) == 0) {
		trim_start(game.response, 2);
	} else if (strncmp(game.response, "now", 3) == 0) {
		trim_start(game.response, 3);
	} else if (strncmp(game.response, "solve", 5) == 0) {
		trim_start(game.response, 5);
	}
}

static void physical_challenge(void) {
	struct physical_challenge delinquent;
	delinquent.health = 2;

	printf("A delinquent appears! They look at you menacingly.\n");

	while (delinquent.health > 0) {
		printf("How do you respond? ");
		ask();

		interpret_input();
		if (strncmp(game.response, "attack", 6) != 0) {
			continue;
		}
		
		delinquent.health--;
		if (delinquent.health > 0) {
			printf("\nThe delinquent takes a hit.\n");
		} else {
			printf("\nThe delinquent falls and dies.\n");
		}
	}
}

static void puzzle_challenge(void) {
	srand(time(NULL));

	struct puzzle_challenge puzzle;
	puzzle.first = rand() % 100 + 1;
	puzzle.second = rand() % 100 + 1;
	size_t answer = puzzle.first * puzzle.second;

	printf("There is a note on the floor. You pick it up.\n");
	printf("It says, '%zu x %zu'.\n", puzzle.first, puzzle.second);

	while (string_to_size_t(game.response) != answer) {
		printf("What could it possibly mean? ");
		ask();
		interpret_input();
	}

	printf("\nYou write '%s' on the note. Nice.\n", game.response);
}

static void clear_challenge(void) {
	size_t i;
	for (i = 0; i < MAX_ROOMS; i++) {
		if (game.player.room.room_number != game.rooms[i].room_number) {
			continue;
		}
		for (size_t j = 0; j < MAX_CHALLENGES_PER_ROOM; j++) {
			if (game.rooms[i].challenge[j] != NONE) {
				game.rooms[i].challenge[j] = NONE;
				game.player.room = game.rooms[i];
				break;
			}
		}
		break;
	}
	if (i == MAX_ROOMS) {
		printf("Cannot clear challenge from a room.\n");
		leave();
	}
}

static void challenge_logic(void) {
	for (size_t i = 0; i < MAX_CHALLENGES_PER_ROOM; i++) {
		switch (game.player.room.challenge[i]) {
		case NONE:
			break;
		case PHYSICAL:
			physical_challenge();
			clear_challenge();
			break;
		case PUZZLE:
			puzzle_challenge();
			clear_challenge();
			break;
		}
	}
}

static void move_logic(size_t next_room) {
	if (next_room == 0) {
		printf("\nYou hit a wall. Ouch!\n");
		return;
	}

	size_t i;
	for (i = 0; i < MAX_ROOMS; i++) {
		if (game.rooms[i].room_number == next_room) {
			game.player.room = game.rooms[i];
			break;
		}
	}
	if (i == MAX_ROOMS) {
		printf("Couldn't find room.\n");
		leave();
	}

	printf("\n%s", game.player.room.message);
}

void game_logic(void) {
	interpret_input();

	size_t next_room = 0;
	bool moved = false;
	if (strncmp(game.response, "help", 4) == 0) {
		printf("\n");
		help_text();
		return;
	} else if (strncmp(game.response, "north", 5) == 0) {
		moved = true;
		next_room = game.player.room.connections[NORTH];
	} else if (strncmp(game.response, "east", 4) == 0) {
		moved = true;
		next_room = game.player.room.connections[EAST];
	} else if (strncmp(game.response, "south", 5) == 0) {
		moved = true;
		next_room = game.player.room.connections[SOUTH];
	} else if (strncmp(game.response, "west", 4) == 0) {
		moved = true;
		next_room = game.player.room.connections[WEST];
	}
	if (moved) {
		move_logic(next_room);
	}

	if (game.player.room.room_number == 1) {
		printf("Congratulations, %s!\n", game.player.name);
		leave();
	}

	challenge_logic();
}
