#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "constant.h"
#include "struct.h"
#include "mutable.h"

#include "utility.h"
#include "file.h"
#include "game.h"

struct game game = {0};

void print_debug_info(void) {
	printf("\n");
	printf("Player name: %s\n", game.player.name);
	printf("Last response: %s\n", game.response);
	printf("Introductory text: %s", game.introductory_text);
	printf("\n");
	printf("[ROOMS]\n");
	printf("\n");

	for (size_t i = 0; i < MAX_ROOMS; i++) {
		if (game.rooms[i].room_number == 0) break;

		printf("Room number: %zu\n", game.rooms[i].room_number);
		printf("Room message: %s\n", game.rooms[i].message);
		printf("North connection: %zu\n", game.rooms[i].connections[NORTH]);
		printf("East connection: %zu\n", game.rooms[i].connections[EAST]);
		printf("South connection: %zu\n", game.rooms[i].connections[SOUTH]);
		printf("West connection: %zu\n", game.rooms[i].connections[WEST]);
		printf("\n");
		for (size_t j = 0; j < MAX_CHALLENGES_PER_ROOM; j++) printf("Challenge %zu: %u\n", j + 1, game.rooms[i].challenge[j]);
		printf("\n");
	}
}

int main(void) {
	load();
	help_text();
	printf("Type 'help' at any time to bring up these instructions.\n\n");

	while (game.response[0] == '\0') {
		printf("What is your name? ");
		ask();
	}

	strncpy(game.player.name, game.response, MAX_RESPONSE_LENGTH);
	game.player.room = game.rooms[0];
	printf("\n%s", game.introductory_text);

	while (1) {
		printf("What would you like to do? ");
		ask();
		game_logic();
		//print_debug_info();
	}

	return 0;
}
