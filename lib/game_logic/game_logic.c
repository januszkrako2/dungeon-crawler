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

#include "utils.h"

void helpText(void) {
	const char* helpText =
	"Type compass directions to move.\n"
	"Type 'attack' to attack.\n"
	"Type numbers to solve puzzles.\n";
	
	printf("%s", helpText);
}

void interpretInput(void) {
	size_t read = 0;
	size_t write = 0;
	while (global.response[read] != '\0') {
		if (isspace(global.response[read])) {
			read++;
			continue;
		}

		// If uppercase, make lowercase
		if (global.response[read] >= 'A' && global.response[read] <= 'Z') global.response[read] += 32;

		global.response[write] = global.response[read];
		write++;
		read++;
	}
	global.response[write] = '\0';

	if (strncmp(global.response, "go", 2) == 0) {
		trimStart(global.response, 2);
	} else if (strncmp(global.response, "now", 3) == 0) {
		trimStart(global.response, 3);
	} else if (strncmp(global.response, "solve", 5) == 0) {
		trimStart(global.response, 5);
	}
}

void physicalChallenge(void) {
	PhysicalChallenge delinquent;
	delinquent.health = 2;

	printf("A delinquent appears! They look at you menacingly.\n");

	while (delinquent.health > 0) {
		printf("How do you respond? ");
		ask();

		interpretInput();
		if (strncmp(global.response, "attack", 6) != 0) continue;
		
		delinquent.health--;
		if (delinquent.health > 0) {
			printf("\nThe delinquent takes a hit.\n");
		} else {
			printf("\nThe delinquent falls and dies.\n");
		}
	}
}

void puzzleChallenge(void) {
	srand(time(NULL));

	PuzzleChallenge puzzle;
	puzzle.firstNumber = rand() % 100 + 1;
	puzzle.secondNumber = rand() % 100 + 1;
	size_t answer = puzzle.firstNumber * puzzle.secondNumber;

	printf("There is a note on the floor. You pick it up.\n");
	printf("It says, '%zu x %zu'.\n", puzzle.firstNumber, puzzle.secondNumber);

	while (stringToSizeT(global.response) != answer) {
		printf("What could it possibly mean? ");
		ask();
		interpretInput();
	}

	printf("\nYou write '%s' on the note. Nice.\n", global.response);
}

void clearChallenge(void) {
	size_t i;
	for (i = 0; i < MAX_ROOMS; i++) {
		if (global.player.currentRoom.roomNumber != global.rooms[i].roomNumber) continue;
		for (size_t j = 0; j < MAX_CHALLENGES_PER_ROOM; j++) {
			if (global.rooms[i].challenge[j] != NONE) {
				global.rooms[i].challenge[j] = NONE;
				global.player.currentRoom = global.rooms[i];
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

void challengeLogic(void) {
	for (size_t i = 0; i < MAX_CHALLENGES_PER_ROOM; i++) {
		switch (global.player.currentRoom.challenge[i]) {
		case NONE:
			break;
		case PHYSICAL:
			physicalChallenge();
			clearChallenge();
			break;
		case PUZZLE:
			puzzleChallenge();
			clearChallenge();
			break;
		}
	}
}

void moveLogic(size_t nextRoom) {
	if (nextRoom == 0) {
		printf("\nYou hit a wall. Ouch!\n");
		return;
	}

	size_t i;
	for (i = 0; i < MAX_ROOMS; i++) {
		if (global.rooms[i].roomNumber == nextRoom) {
			global.player.currentRoom = global.rooms[i];
			break;
		}
	}
	if (i == MAX_ROOMS) {
		printf("Couldn't find room.\n");
		leave();
	}

	printf("\n%s", global.player.currentRoom.message);
}

void gameLogic(void) {
	interpretInput();

	size_t nextRoom = 0;
	bool moved = false;
	if (strncmp(global.response, "help", 4) == 0) {
		printf("\n");
		helpText();
		return;
	} else if (strncmp(global.response, "north", 5) == 0) {
		moved = true;
		nextRoom = global.player.currentRoom.connections[NORTH];
	} else if (strncmp(global.response, "east", 4) == 0) {
		moved = true;
		nextRoom = global.player.currentRoom.connections[EAST];
	} else if (strncmp(global.response, "south", 5) == 0) {
		moved = true;
		nextRoom = global.player.currentRoom.connections[SOUTH];
	} else if (strncmp(global.response, "west", 4) == 0) {
		moved = true;
		nextRoom = global.player.currentRoom.connections[WEST];
	}
	if (moved) moveLogic(nextRoom);

	if (global.player.currentRoom.roomNumber == 1) {
		printf("Congratulations, %s!\n", global.player.name);
		leave();
	}

	challengeLogic();
}
