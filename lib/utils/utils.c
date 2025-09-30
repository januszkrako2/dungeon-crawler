#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "constants.h"
#include "structs.h"
#include "mutables.h"

void leave(void) {
	while (strncmp(global.response, "\n", 1) != 0) {
		printf("Press enter to leave.\n");

		if (fgets(global.response, MAX_RESPONSE_LENGTH, stdin) == NULL) {
			perror("Error getting user input.");
			exit(1);
		}

		char* newlinePosition = strchr(global.response, '\n');
		if (newlinePosition == NULL) {
			while(getchar() != '\n');
		}
	}
	exit(1);
}

void ask(void) {
	if (fgets(global.response, MAX_RESPONSE_LENGTH, stdin) == NULL) {
		perror("Error getting user input.");
		leave();
	}

	char* newlinePosition = strchr(global.response, '\n');
	if (newlinePosition == NULL) {
		while(getchar() != '\n');
	} else {
		*newlinePosition = '\0';
	}
}

void trimStart(char* text, size_t characters) {
	size_t i = 0;
	while (text[i + characters] != '\0') {
		text[i] = text[i + characters];
		i++;
	}
	for (size_t j = 0; j < characters; j++) {
		text[i + j] = '\0';
	}
}

size_t stringToSizeT(char* text) {
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
