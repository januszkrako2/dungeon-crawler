#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "defines.h"
#include "structs.h"
#include "externs.h"
#include "text_utils.h"

void initialise(FILE* saveFile) {
	const char* initialConfiguration =
		"[ROOMS]\n"
		"\n"
		"ROOM NUMBER: 100\n"
		"MESSAGE: You enter a dark room. A light glimmers up north.\n"
		"CONNECTIONS:\n"
		"\tNORTH: 101\n"
		"\tEAST: None\n"
		"\tSOUTH: None\n"
		"\tWEST: None\n"
		"CHALLENGE: None\n"
		"\n"
		"ROOM NUMBER: 101\n"
		"MESSAGE: You enter a dingy living room. It is not well-kept.\n"
		"CONNECTIONS:\n"
		"\tNORTH: None\n"
		"\tEAST: None\n"
		"\tSOUTH: 100\n"
		"\tWEST: None\n"
		"CHALLENGE: Physical\n"
		"\n"
		"[HISTORY]\n"
		"\n"
		"[GAME STATE]\n"
		"\n"
		"PLAYER NAME: \n"
		"CURRENT ROOM OF PLAYER: \n"
		"ROOMS WITH COMPLETED CHALLENGES: \n";
	fputs(initialConfiguration, saveFile);
	rewind(saveFile);
}

void extractData(FILE* saveFile) {
	uint8_t roomCounter = 0;
	bool connectingRooms = false;
	size_t lineCounter = 1;
	uint8_t lineCharacterCounter = 0;
	char line[80] = {0};
	char current;
	while ((current = fgetc(saveFile)) != EOF) {
		if (lineCharacterCounter > sizeof(line)) {
			perror("One of your lines in savefile.txt is too long");
			exit(1);
		}
		line[lineCharacterCounter] = current;
		lineCharacterCounter++;

		if (roomCounter == MAX_ROOMS) {
			perror("Error: too many rooms in savefile.txt");
			exit(1);
		}

		// Extract room number
		if (lineCounter % 9 == 3 && current == '\n') {
			if (strncmp(line, "ROOM NUMBER: ", 13) == 0) {
				trimStart(line, 13);
				size_t roomNumberConverted = 
					stringToSizeT(line);
				rooms[roomCounter].roomNumber = 
					roomNumberConverted;
			} else {
				return;
			}
		}

		// Extract message
		if (lineCounter % 9 == 4 && current == '\n') {
			if (strncmp(line, "MESSAGE: ", 9) == 0) {
				trimStart(line, 9);
				for (size_t i = 0; i < MAX_ROOM_MESSAGE_LENGTH;
						i++) {
					rooms[roomCounter].message[i] =
						line[i];
					if (line[i] == '\0') {
						break;
					}
				}
			}
		}

		// Extract connections
		if (lineCounter % 9 == 5 && current == '\n') {
			if (strncmp(line, "CONNECTIONS:", 12) == 0) {
				connectingRooms = true;
			}
		}
		if (connectingRooms && current == '\n') {
			switch (lineCounter % 9) {
				case 6:
				if (strncmp(line, "\tNORTH: ", 8) == 0) {
					trimStart(line, 8);
					rooms[roomCounter].connections[NORTH] =
						stringToSizeT(line);
				}
				break;
				case 7:
				if (strncmp(line, "\tEAST: ", 7) == 0) {
					trimStart(line, 7);
					rooms[roomCounter].connections[EAST] =
						stringToSizeT(line);
				}
				break;
				case 8:
				if (strncmp(line, "\tSOUTH: ", 8) == 0) {
					trimStart(line, 8);
					rooms[roomCounter].connections[SOUTH] =
						stringToSizeT(line);
				} 
				break;
				case 0:
				if (strncmp(line, "\tWEST: ", 7) == 0) {
					trimStart(line, 7);
					rooms[roomCounter].connections[WEST] =
						stringToSizeT(line);
					connectingRooms = false;
				}
				break;
			}
		}

		// Extract challenges
		if (lineCounter % 9 == 1 && lineCounter > 1 &&
				current == '\n') {
			if (strncmp(line, "CHALLENGE: ", 11) == 0) {
				trimStart(line, 11);
				uint8_t roomChallengeCounter = 0;
				while (line[0] != '\n') {
					if (strncmp(line, "None", 4) == 0) {
						trimStart(line, 4);
					} else
					if (strncmp(line, "Physical", 8) == 0) {
						trimStart(line, 8);
						rooms[roomCounter]
					.challenge[roomChallengeCounter] =
							PHYSICAL;
						roomChallengeCounter++;
					} else
					if (strncmp(line, "Puzzle", 6) == 0) {
						trimStart(line, 6);
						rooms[roomCounter]
					.challenge[roomChallengeCounter] =
							PUZZLE;
						roomChallengeCounter++;
					} else
					if (strncmp(line, ", ", 2) == 0) {
						trimStart(line, 2);
					} else {
						memset(line, 0, sizeof(line));
						line[0] = '\n';
					}
				}
				if (roomChallengeCounter >
						MAX_CHALLENGES_PER_ROOM) {
					fprintf(stderr,
						"Too many challenges assigned to room %lu (max %u).\n",
					rooms[roomCounter].roomNumber, MAX_CHALLENGES_PER_ROOM);
					exit(1);
				}
				roomChallengeCounter = 0;
				roomCounter++;
			}
		}
		
		// Extract game state
		if (current == '\n' && strncmp(line, "PLAYER NAME: ", 13) == 0) {
			trimStart(line, 13);
			for (uint8_t i = 0; i < PLAYER_NAME_LENGTH; i++) {
				if (line[i] == '\n') {
					break;
				}
				player.name[i] = line[i];
			}
		}
		if (current == '\n'
					&& strncmp(line, "CURRENT ROOM OF PLAYER: ", 24) == 0) {
			trimStart(line, 24);
			size_t currentRoomOfPlayer = stringToSizeT(line);
			if (currentRoomOfPlayer == 0) {
				fprintf(stderr,
					"Room of player cannot be 0 (savefile line %lu).\n",
					lineCounter);
				exit(1);
			} else {
				size_t i;
				for (i = 0; i < MAX_ROOMS; i++) {
					if (rooms[i].roomNumber == currentRoomOfPlayer) {
						player.currentRoom = rooms[i];
						break;
					}
				}
				if (i == MAX_ROOMS) {
					fprintf(stderr,
						"Invalid player room number (savefile line %lu).\n",
						lineCounter);
					exit(1);
				}
			}
		}
		if (current == '\n' &&
						strncmp(line, 
								"ROOMS WITH COMPLETED CHALLENGES: ",
								33) == 0) {
			trimStart(line, 33);
			size_t lineLength = sizeof(line);
			char buffer[lineLength];
			for (size_t i = 0; i < lineLength; i++) {
				buffer[i] = 0;
			}
			size_t bufferCounter = 0;
			for (size_t i = 0; i < lineLength; i++) {
				if (line[i] == '\n') {
					break;
				}
				buffer[bufferCounter] = line[i];
				if (buffer[bufferCounter] == ',') {
					size_t currentRoomNumber = stringToSizeT(buffer);
					if (currentRoomNumber == 0) {
						perror("Invalid completed room number in savefile.");
						exit(1);
					}
					size_t j;
					for (j = 0; j < MAX_ROOMS; j++) {
						if (currentRoomNumber == rooms[j].roomNumber) {
							for (size_t k = 0;
										k < MAX_CHALLENGES_PER_ROOM;
										k++) {
								rooms[j].challenge[k] = NONE;
							}
							break;
						}
					}
					if (j == MAX_ROOMS) {
						perror("A completed room number doesn't"
							   "match a room number in the savefile."
						);
						exit(1);
					}
					memset(buffer, 0, lineLength);
					bufferCounter = 0;
					continue;
				}
				bufferCounter++;
			}
		}
		
		// Update line
		if (current == '\n') {
			lineCounter++;
			lineCharacterCounter = 0;
			memset(line, 0, sizeof(line));
		}
	}
}

void load() {
	FILE* saveFile = fopen("savefile.txt", "r+");
	if (saveFile == NULL) {
		saveFile = fopen("savefile.txt", "w+");
		if (saveFile == NULL) {
			perror("Error creating save file.");
			exit(1);
		}
		initialise(saveFile);
	}
	extractData(saveFile);
	fclose(saveFile);
}

void saveHistory(FILE* saveFile) {
	char line[80] = {0};
	size_t lineCounter = 0;
	uint8_t lineCharacterCounter = 0;
	char current;
	while((current = fgetc(saveFile)) != EOF) {
		if (lineCharacterCounter > sizeof(line)) {
			perror("One of your lines in savefile.txt is too long");
			exit(1);
		}
		line[lineCharacterCounter] = current;
		lineCharacterCounter++;
		if (current == '\n') {
			lineCounter++;
			lineCharacterCounter = 0;
			memset(line, 0, sizeof(line));
		}
	}
}

void saveData(FILE* saveFile) {
	char current;
	while ((current = fgetc(saveFile)) != EOF) {
		// TODO: logic for saving the game
	}
}

void save() {
	FILE* saveFile = fopen("savefile.txt", "w+");
	if (saveFile == NULL) {
		perror("Error saving to file.");
		exit(1);
	}
	saveData(saveFile);
	fclose(saveFile);
}
