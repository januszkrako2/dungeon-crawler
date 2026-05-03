#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "constant.h"
#include "struct.h"
#include "mutable.h"

void leave(void) {
	while (strncmp(game.response, "\n", 1) != 0) {
		printf("Press enter to leave.\n");

		if (fgets(game.response, MAX_RESPONSE_LENGTH, stdin) == NULL) {
			printf("Error getting user input.\n");
			exit(1);
		}

		char* newline_position = strchr(game.response, '\n');
		if (newline_position == NULL) {
			while(getchar() != '\n');
		}
	}

	exit(1);
}

void ask(void) {
	if (fgets(game.response, MAX_RESPONSE_LENGTH, stdin) == NULL) {
		printf("Error getting user input.\n");
		leave();
	}

	char* newline_position = strchr(game.response, '\n');

	if (newline_position == NULL) {
		while (getchar() != '\n');
	} else {
		*newline_position = '\0';
	}
}

void trim_start(char* text, size_t characters) {
	size_t i = 0;

	while (text[i + characters] != '\0') {
		text[i] = text[i + characters];
		i++;
	}

	for (size_t j = 0; j < characters; j++) {
		text[i + j] = '\0';
	}
}

size_t string_to_size_t(char* text) {
	size_t value = 0;
	size_t i = 0;

	while (text[i] != '\0') {
		if (text[i] >= '0' && text[i] <= '9') {
			value = (value * 10) + text[i] - '0';
		}
		i++;
	}

	return value;
}
