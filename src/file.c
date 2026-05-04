#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "constant.h"
#include "struct.h"
#include "mutable.h"

#include "utility.h"

struct file {
	bool is_connecting_rooms;
	bool is_reading_introductory_text;
	size_t room_counter;
	size_t room_challenge_counter;
	size_t line_counter;
	size_t line_character_counter;
	char line[MAX_FILE_LINE_LENGTH];
	char current;
	bool errored;
};

static void initialise_room_file(FILE* room_file) {
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

static void error_out(struct file* file) {
	if (file->line_character_counter > MAX_FILE_LINE_LENGTH) {
		printf("Error: line %zu of rooms.txt is too long (max %d characters, currently %zu).\n",
		       file->line_counter, MAX_FILE_LINE_LENGTH, file->line_character_counter);
		leave();
	}
	if (file->room_counter >= MAX_ROOMS) {
		printf("Error: too many rooms in rooms.txt (max %d).\n", MAX_ROOMS);
		leave();
	}
	if (file->room_challenge_counter > MAX_CHALLENGES_PER_ROOM) {
		size_t currentRoom = game.rooms[file->room_counter].room_number;
		printf("Error: too many challenges assigned to room %zu (line %zu, max %u, currently %zu).\n",
		       currentRoom, file->line_counter, MAX_CHALLENGES_PER_ROOM, file->room_challenge_counter);
		leave();
	}
}

static void extract_room_number(struct file* file) {
	if (file->errored) {
		return;
	}
	if (file->line_counter % 9 != 3) {
		return;
	}
	if (file->current != '\n') {
		return;
	}

	if (strncmp(file->line, "ROOM NUMBER: ", 13) == 0) {
		trim_start(file->line, 13);
		size_t extracted = string_to_size_t(file->line);
		game.rooms[file->room_counter].room_number = extracted;
	}
}

static void extract_room_message(struct file* file) {
	if (file->errored) {
		return;
	}
	if (file->line_counter % 9 != 4) {
		return;
	}
	if (file->current != '\n') {
		return;
	}
	if (strncmp(file->line, "MESSAGE: ", 9) != 0) {
		return;
	}
	
	trim_start(file->line, 9);
	strcpy(game.rooms[file->room_counter].message, file->line);
}

static void connecting_room_check(struct file* file) {
	if (file->errored) {
		return;
	}
	if (file->current != '\n') {
		return;
	}
	if (file->line_counter % 9 != 5) {
		return;
	}
	if (strncmp(file->line, "CONNECTIONS:", 12) != 0) {
		return;
	}

	file->is_connecting_rooms = true;
}

static void add_room_connection(struct connection* connection, struct file* file, enum direction direction) {
	if (strncmp(file->line, connection->text, connection->size) != 0) {
		return;
	}

	trim_start(file->line, connection->size);
	game.rooms[file->room_counter].connections[direction] = string_to_size_t(file->line);
}

static void extract_room_connections(struct file* file) {
	if (file->errored) {
		return;
	}
	if (file->current != '\n') {
		return;
	}
	if (file->is_connecting_rooms != true) {
		return;
	}

	struct connection connection = {0};
	switch (file->line_counter % 9) {
	case 6:
		connection.text = "\tNORTH: ";
		connection.size = 8;
		add_room_connection(&connection, file, NORTH);
		break;
	case 7:
		connection.text = "\tEAST: ";
		connection.size = 7;
		add_room_connection(&connection, file, EAST);
		break;
	case 8:
		connection.text = "\tSOUTH: ";
		connection.size = 8;
		add_room_connection(&connection, file, SOUTH);
		break;
	case 0:
		connection.text = "\tWEST: ";
		connection.size = 7;
		add_room_connection(&connection, file, WEST);
		file->is_connecting_rooms = false;
		break;
	}
}

static void add_room_challenges(struct file* file) {
	char* line = file->line;
	size_t room_index = file->room_counter;
	if (strncmp(line, "None", 4) == 0) {
		trim_start(line, 4);
	} else if (strncmp(line, "Physical", 8) == 0) {
		trim_start(line, 8);
		game.rooms[room_index].challenge[file->room_challenge_counter] = PHYSICAL;
		file->room_challenge_counter++;
	} else if (strncmp(line, "Puzzle", 6) == 0) {
		trim_start(line, 6);
		game.rooms[room_index].challenge[file->room_challenge_counter] = PUZZLE;
		file->room_challenge_counter++;
	} else if (strncmp(line, ", ", 2) == 0) {
		trim_start(line, 2);
	} else {
		line[0] = '\n';
	}
}

static void extract_room_challenges(struct file* file) {
	if (file->errored) {
		return;
	}
	if (file->line_counter % 9 != 1) {
		return;
	}
	if (file->line_counter <= 1) {
		return;
	}
	if (file->current != '\n') {
		return;
	}
	if (strncmp(file->line, "CHALLENGE: ", 11) != 0) {
		return;
	}

	trim_start(file->line, 11);

	while (file->line[0] != '\n') {
		add_room_challenges(file);
	}

	if (file->room_challenge_counter > MAX_CHALLENGES_PER_ROOM) {
		file->errored = true;
		return;
	}

	file->room_challenge_counter = 0;
	file->room_counter++;
}

static void introductory_text_check(struct file* file) {
	if (file->errored) {
		return;
	}
	if (file->current != '\n') {
		return;
	}
	if (file->line[0] == '\n') {
		return;
	}
	if (strncmp(file->line, "[INTRODUCTORY TEXT]", 19) != 0) {
		return;
	}

	file->is_reading_introductory_text = true;
}

static void extract_introductory_text(struct file* file) {
	if (file->errored) {
		return;
	}
	if (file->is_reading_introductory_text != true) {
		return;
	}

	size_t i = 0;
	while (file->line[i] != '\0') {
		game.introductory_text[i] = file->line[i];
		i++;
	}
}

static void update_line(struct file* file) {
	if (file->current != '\n') {
		return;
	}

	if (file->errored) {
		error_out(file);
	}

	file->line_counter++;
	file->line_character_counter = 0;
	memset(file->line, 0, MAX_FILE_LINE_LENGTH);
}

static void extract(FILE* room_file) {
	struct file file = {0};
	file.line_counter = 1;

	while ((file.current = fgetc(room_file)) != EOF) {
		file.line[file.line_character_counter] = file.current;
		file.line_character_counter++;

		if (file.line_character_counter >= MAX_FILE_LINE_LENGTH) {
			file.errored = true;
		}
		if (file.room_counter >= MAX_ROOMS) {
			file.errored = true;
		}

		extract_room_number(&file);
		extract_room_message(&file);
		connecting_room_check(&file);
		extract_room_connections(&file);
		extract_room_challenges(&file);
		introductory_text_check(&file);
		extract_introductory_text(&file);
		update_line(&file);
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
