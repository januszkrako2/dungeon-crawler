#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "constant.h"
#include "struct.h"
#include "mutable.h"

#include "utility.h"

void initialise_room_file(FILE* room_file) {
	const char* initial_configuration =
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

	fputs(initial_configuration, room_file);
	rewind(room_file);
}

void error_check(struct file* info) {
	if (info->line_character_counter >= MAX_FILE_LINE_LENGTH) {
		info->errored = true;
	}
	if (info->room_counter >= MAX_ROOMS) {
		info->errored = true;
	}
}

void error_out(struct file* info) {
	if (info->line_character_counter > MAX_FILE_LINE_LENGTH) {
		printf("Error: line %zu of rooms.txt is too long (max %d characters, currently %zu).\n", 
		       info->line_counter, MAX_FILE_LINE_LENGTH, info->line_character_counter);
		leave();
	}
	if (info->room_counter >= MAX_ROOMS) {
		printf("Error: too many rooms in rooms.txt (max %d).\n", MAX_ROOMS);
		leave();
	}
	if (info->room_challenge_counter > MAX_CHALLENGES_PER_ROOM) {
		size_t currentRoom = game.rooms[info->room_counter].room_number;
		printf("Error: too many challenges assigned to room %zu (line %zu, max %u, currently %zu).\n",
		       currentRoom, info->line_counter, MAX_CHALLENGES_PER_ROOM, info->room_challenge_counter);
		leave();
	}
}

void extract_room_number(struct file* info) {
	if (info->errored) {
		return;
	}
	if (info->line_counter % 9 != 3) {
		return;
	}
	if (info->current != '\n') {
		return;
	}

	if (strncmp(info->line, "ROOM NUMBER: ", 13) == 0) {
		trim_start(info->line, 13);
		size_t extracted = string_to_size_t(info->line);
		game.rooms[info->room_counter].room_number = extracted;
	}
}

void extract_room_message(struct file* info) {
	if (info->errored) {
		return;
	}
	if (info->line_counter % 9 != 4) {
		return;
	}
	if (info->current != '\n') {
		return;
	}
	if (strncmp(info->line, "MESSAGE: ", 9) != 0) {
		return;
	}
	
	trim_start(info->line, 9);
	strcpy(game.rooms[info->room_counter].message, info->line);
}

void connecting_room_check(struct file* info) {
	if (info->errored) {
		return;
	}
	if (info->current != '\n') {
		return;
	}
	if (info->line_counter % 9 != 5) {
		return;
	}
	if (strncmp(info->line, "CONNECTIONS:", 12) != 0) {
		return;
	}

	info->is_connecting_rooms = true;
}

void add_room_connection(struct connection* connection, struct file* info, enum direction direction) {
	if (strncmp(info->line, connection->text, connection->size) != 0) {
		return;
	}

	trim_start(info->line, connection->size);
	game.rooms[info->room_counter].connections[direction] = string_to_size_t(info->line);
}

void extract_room_connections(struct file* info) {
	if (info->errored) {
		return;
	}
	if (info->current != '\n') {
		return;
	}
	if (info->is_connecting_rooms != true) {
		return;
	}

	struct connection connection = {0};
	switch (info->line_counter % 9) {
	case 6:
		connection.text = "\tNORTH: ";
		connection.size = 8;
		add_room_connection(&connection, info, NORTH);
		break;
	case 7:
		connection.text = "\tEAST: ";
		connection.size = 7;
		add_room_connection(&connection, info, EAST);
		break;
	case 8:
		connection.text = "\tSOUTH: ";
		connection.size = 8;
		add_room_connection(&connection, info, SOUTH);
		break;
	case 0:
		connection.text = "\tWEST: ";
		connection.size = 7;
		add_room_connection(&connection, info, WEST);
		info->is_connecting_rooms = false;
		break;
	}
}

void add_room_challenges(struct file* info) {
	char* line = info->line;
	size_t room_index = info->room_counter;
	if (strncmp(line, "None", 4) == 0) {
		trim_start(line, 4);
	} else if (strncmp(line, "Physical", 8) == 0) {
		trim_start(line, 8);
		game.rooms[room_index].challenge[info->room_challenge_counter] = PHYSICAL;
		info->room_challenge_counter++;
	} else if (strncmp(line, "Puzzle", 6) == 0) {
		trim_start(line, 6);
		game.rooms[room_index].challenge[info->room_challenge_counter] = PUZZLE;
		info->room_challenge_counter++;
	} else if (strncmp(line, ", ", 2) == 0) {
		trim_start(line, 2);
	} else {
		line[0] = '\n';
	}
}

void extract_room_challenges(struct file* info) {
	if (info->errored) {
		return;
	}
	if (info->line_counter % 9 != 1) {
		return;
	}
	if (info->line_counter <= 1) {
		return;
	}
	if (info->current != '\n') {
		return;
	}
	if (strncmp(info->line, "CHALLENGE: ", 11) != 0) {
		return;
	}

	trim_start(info->line, 11);

	while (info->line[0] != '\n') {
		add_room_challenges(info);
	}

	if (info->room_challenge_counter > MAX_CHALLENGES_PER_ROOM) {
		info->errored = true;
		return;
	}

	info->room_challenge_counter = 0;
	info->room_counter++;
}

void introductory_text_check(struct file* info) {
	if (info->errored) {
		return;
	}
	if (info->current != '\n') {
		return;
	}
	if (info->line[0] == '\n') {
		return;
	}
	if (strncmp(info->line, "[INTRODUCTORY TEXT]", 19) != 0) {
		return;
	}

	info->is_reading_introductory_text = true;
}

void extract_introductory_text(struct file* info) {
	if (info->errored) {
		return;
	}
	if (info->is_reading_introductory_text != true) {
		return;
	}

	size_t i = 0;
	while (info->line[i] != '\0') {
		game.introductory_text[i] = info->line[i];
		i++;
	}
}

void update_line(struct file* info) {
	if (info->current != '\n') {
		return;
	}

	if (info->errored) {
		error_out(info);
	}

	info->line_counter++;
	info->line_character_counter = 0;
	memset(info->line, 0, MAX_FILE_LINE_LENGTH);
}

void extract(FILE* room_file) {
	struct file info = {0};
	info.line_counter = 1;

	while ((info.current = fgetc(room_file)) != EOF) {
		info.line[info.line_character_counter] = info.current;
		info.line_character_counter++;
		error_check(&info);
		extract_room_number(&info);
		extract_room_message(&info);
		connecting_room_check(&info);
		extract_room_connections(&info);
		extract_room_challenges(&info);
		introductory_text_check(&info);
		extract_introductory_text(&info);
		update_line(&info);
	}
}

void load(void) {
	FILE* room_file = fopen("rooms.txt", "r+");
	if (room_file != NULL) {
		extract(room_file);
		fclose(room_file);
		return;
	}

	room_file = fopen("rooms.txt", "w+");
	if (room_file == NULL) {
		printf("Error: Cannot create room file.\n");
		leave();
	}

	initialise_room_file(room_file);
	extract(room_file);
	fclose(room_file);
}
