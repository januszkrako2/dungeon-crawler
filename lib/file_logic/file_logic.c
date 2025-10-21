#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "constants.h"
#include "structs.h"
#include "mutables.h"

#include "utils.h"

void initialiseRoomFile(FILE* roomFile) {
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
	"\tEAST: 1\n"
	"\tSOUTH: 100\n"
	"\tWEST: None\n"
	"CHALLENGE: Physical, Puzzle\n"
	"\n"
	"ROOM NUMBER: 1\n"
	"MESSAGE: You enter the winning room.\n"
	"CONNECTIONS:\n"
	"\tNORTH: None\n"
	"\tEAST: None\n"
	"\tSOUTH: None\n"
	"\tWEST: None\n"
	"CHALLENGE: None\n"
	"\n"
	"[INTRODUCTORY TEXT]\n"
	"\n"
	"You arise from a deep rest. There's a door up north.\n";

	fputs(initialConfiguration, roomFile);
	rewind(roomFile);
}

void validateLine(FileInfo* info) {
	if (info->lineCharacterCounter > MAX_FILE_LINE_LENGTH) {
		printf("Error: line %zu in rooms.txt is too long (max %d characters, currently %zu).\n",
		       info->lineCounter, MAX_FILE_LINE_LENGTH, info->lineCharacterCounter);
		leave();
	}
	if (info->roomCounter >= MAX_ROOMS) {
		printf("Error: too many rooms in rooms.txt (max %d).\n", MAX_ROOMS);
		leave();
	}
}

void addRoomNumber(FileInfo* info) {
	size_t extracted = stringToSizeT(info->line);
	global.rooms[info->roomCounter].roomNumber = extracted;
}

void extractRoomNumber(FileInfo* info) {
	if (info->lineCounter % 9 != 3) return;
	if (info->current != '\n') return;

	if (strncmp(info->line, "ROOM NUMBER: ", 13) == 0) {
		trimStart(info->line, 13);
		addRoomNumber(info);
	}
}

void extractRoomMessage(FileInfo* info) {
	if (info->lineCounter % 9 != 4) return;
	if (info->current != '\n') return;
	if (strncmp(info->line, "MESSAGE: ", 9) != 0) return;
	
	trimStart(info->line, 9);
	strcpy(global.rooms[info->roomCounter].message, info->line);
}

void connectingRoomCheck(FileInfo* info) {
	if (info->current != '\n') return;
	if (info->lineCounter % 9 != 5) return;
	if (strncmp(info->line, "CONNECTIONS:", 12) != 0) return;

	info->connectingRooms = true;
}

void addRoomConnection(Connection* connection, FileInfo* info, Direction direction) {
	if (strncmp(info->line, connection->text, connection->size) != 0) return;
	trimStart(info->line, connection->size);
	global.rooms[info->roomCounter].connections[direction] = stringToSizeT(info->line);
}

void extractRoomConnections(FileInfo* info) {
	if (info->current != '\n') return;
	if (info->connectingRooms != true) return;

	Connection connection = {0};
	switch (info->lineCounter % 9) {
	case 6:
		connection.text = "\tNORTH: ";
		connection.size = 8;
		addRoomConnection(&connection, info, NORTH);
		break;
	case 7:
		connection.text = "\tEAST: ";
		connection.size = 7;
		addRoomConnection(&connection, info, EAST);
		break;
	case 8:
		connection.text = "\tSOUTH: ";
		connection.size = 8;
		addRoomConnection(&connection, info, SOUTH);
		break;
	case 0:
		connection.text = "\tWEST: ";
		connection.size = 7;
		addRoomConnection(&connection, info, WEST);
		info->connectingRooms = false;
		break;
	}
}

void addRoomChallenges(FileInfo* info, size_t* challengeCounter) {
	char* line = info->line;
	size_t roomIndex = info->roomCounter;
	size_t* challengeIndex = challengeCounter;

	if (strncmp(line, "None", 4) == 0) trimStart(line, 4);
	else if (strncmp(line, "Physical", 8) == 0) {
		trimStart(line, 8);
		global.rooms[roomIndex].challenge[(*challengeIndex)] = PHYSICAL;
		(*challengeIndex)++;
	} else if (strncmp(line, "Puzzle", 6) == 0) {
		trimStart(line, 6);
		global.rooms[roomIndex].challenge[(*challengeIndex)] = PUZZLE;
		(*challengeIndex)++;
	} else if (strncmp(line, ", ", 2) == 0) trimStart(line, 2);
	else line[0] = '\n';
}

void extractRoomChallenges(FileInfo* info) {
	if (info->lineCounter % 9 != 1) return;
	if (info->lineCounter <= 1) return;
	if (info->current != '\n') return;
	if (strncmp(info->line, "CHALLENGE: ", 11) != 0) return;

	trimStart(info->line, 11);

	size_t roomChallengeCounter = 0;
	while (info->line[0] != '\n') addRoomChallenges(info, &roomChallengeCounter);

	if (roomChallengeCounter <= MAX_CHALLENGES_PER_ROOM) {
		roomChallengeCounter = 0;
		info->roomCounter++;
		return;
	}

	size_t currentRoom = global.rooms[info->roomCounter].roomNumber;
	printf("Too many challenges assigned to room %lu (max %u).\n", currentRoom, MAX_CHALLENGES_PER_ROOM);
	leave();
}

void introductoryTextCheck(FileInfo* info) {
	if (info->current != '\n') return;
	if (info->line[0] == '\n') return;
	if (strncmp(info->line, "[INTRODUCTORY TEXT]", 19) != 0) return;

	info->readingIntroductoryText = true;
}

void extractIntroductoryText(FileInfo* info) {
	if (info->readingIntroductoryText != true) return;

	size_t i = 0;
	while (info->line[i] != '\0') {
		global.introductoryText[i] = info->line[i];
		i++;
	}
}

void updateLine(FileInfo* info) {
	if (info->current != '\n') return;
	
	validateLine(info);

	info->lineCounter++;
	info->lineCharacterCounter = 0;
	memset(info->line, 0, MAX_FILE_LINE_LENGTH);
}

void extract(FILE* roomFile) {
	FileInfo info = {0};
	info.lineCounter = 1;

	while ((info.current = fgetc(roomFile)) != EOF) {
		info.line[info.lineCharacterCounter] = info.current;
		info.lineCharacterCounter++;
		extractRoomNumber(&info);
		extractRoomMessage(&info);
		connectingRoomCheck(&info);
		extractRoomConnections(&info);
		extractRoomChallenges(&info);
		introductoryTextCheck(&info);
		extractIntroductoryText(&info);
		updateLine(&info);
	}
}

void load(void) {
	FILE* roomFile = fopen("rooms.txt", "r+");
	if (roomFile != NULL) {
		extract(roomFile);
		fclose(roomFile);
		return;
	}

	roomFile = fopen("rooms.txt", "w+");
	if (roomFile == NULL) {
		printf("Error creating room file.\n");
		leave();
	}

	initialiseRoomFile(roomFile);
	extract(roomFile);
	fclose(roomFile);
}
