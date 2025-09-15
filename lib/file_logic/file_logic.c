#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "defines.h"
#include "structs.h"
#include "externs.h"
#include "text_utils.h"

void initialiseRoomFile(FILE* roomFile)
{
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
		"CHALLENGE: Physical\n";
	
	fputs(initialConfiguration, roomFile);
	rewind(roomFile);
}

void extractRooms(FILE* roomFile)
{
	uint8_t roomCounter = 0;
	bool connectingRooms = false;
	size_t lineCounter = 1;
	uint8_t lineCharacterCounter = 0;
	char line[80] = {0};
	size_t lineSize = sizeof(line);
	char current;

	while ((current = fgetc(roomFile)) != EOF)
	{
		if (lineCharacterCounter > sizeof(line))
		{
			perror("One of your lines in savefile.txt is too long");
			exit(1);
		}

		line[lineCharacterCounter] = current;
		lineCharacterCounter++;

		if (roomCounter == MAX_ROOMS)
		{
			perror("Error: too many rooms in savefile.txt");
			exit(1);
		}

		// Extract room number
		if (lineCounter % 9 == 3 && current == '\n')
		{
			if (strncmp(line, "ROOM NUMBER: ", 13) == 0)
			{
				trimStart(line, 13);
				size_t roomNumberConverted = 
					stringToSizeT(line);
				rooms[roomCounter].roomNumber = 
					roomNumberConverted;
			}
			else
			{
				return;
			}
		}

		// Extract message
		if (lineCounter % 9 == 4 && current == '\n')
		{
			if (strncmp(line, "MESSAGE: ", 9) == 0)
			{
				trimStart(line, 9);

				for (size_t i = 0; i < MAX_ROOM_MESSAGE_LENGTH; i++)
				{
					rooms[roomCounter].message[i] = line[i];

					if (line[i] == '\0')
					{
						break;
					}
				}
			}
		}

		// Extract connections
		if (lineCounter % 9 == 5 && current == '\n')
		{
			if (strncmp(line, "CONNECTIONS:", 12) == 0)
			{
				connectingRooms = true;
			}
		}

		if (connectingRooms && current == '\n')
		{
			switch (lineCounter % 9)
			{
				case 6:
				{
					if (strncmp(line, "\tNORTH: ", 8) == 0)
					{
						trimStart(line, 8);
						rooms[roomCounter].connections[NORTH] =
							stringToSizeT(line);
					}

					break;
				}

				case 7:
				{
					if (strncmp(line, "\tEAST: ", 7) == 0)
					{
						trimStart(line, 7);
						rooms[roomCounter].connections[EAST] =
							stringToSizeT(line);
					}
					
					break;
				}

				case 8:
				{
					if (strncmp(line, "\tSOUTH: ", 8) == 0)
					{
						trimStart(line, 8);
						rooms[roomCounter].connections[SOUTH] =
							stringToSizeT(line);
					}

					break;
				}
				
				case 0:
				{
					if (strncmp(line, "\tWEST: ", 7) == 0)
					{
						trimStart(line, 7);
						rooms[roomCounter].connections[WEST] =
							stringToSizeT(line);
						connectingRooms = false;
					}

					break;
				}
			}
		}

		// Extract challenges
		if (lineCounter % 9 == 1 && lineCounter > 1 && current == '\n')
		{
			if (strncmp(line, "CHALLENGE: ", 11) == 0)
			{
				trimStart(line, 11);
				uint8_t roomChallengeCounter = 0;

				while (line[0] != '\n')
				{
					if (strncmp(line, "None", 4) == 0)
					{
						trimStart(line, 4);
					}
					else if (strncmp(line, "Physical", 8) == 0)
					{
						trimStart(line, 8);
						rooms[roomCounter].challenge[roomChallengeCounter] =
							PHYSICAL;
						roomChallengeCounter++;
					}
					else if (strncmp(line, "Puzzle", 6) == 0)
					{
						trimStart(line, 6);
						rooms[roomCounter].challenge[roomChallengeCounter] =
							PUZZLE;
						roomChallengeCounter++;
					}
					else if (strncmp(line, ", ", 2) == 0)
					{
						trimStart(line, 2);
					}
					else
					{
						memset(line, 0, sizeof(line));
						line[0] = '\n';
					}
				}

				if (roomChallengeCounter > MAX_CHALLENGES_PER_ROOM)
				{
					fprintf(
						stderr,
						"Too many challenges assigned to room %lu (max %u).\n",
						rooms[roomCounter].roomNumber, MAX_CHALLENGES_PER_ROOM
					);
					exit(1);
				}

				roomChallengeCounter = 0;
				roomCounter++;
			}
		}
		
		// Update line
		if (current == '\n')
		{
			lineCounter++;
			lineCharacterCounter = 0;
			memset(line, 0, lineSize);
		}
	}
}

void load(void)
{
	FILE* roomFile = fopen("rooms.txt", "r+");

	if (roomFile == NULL)
	{
		roomFile = fopen("rooms.txt", "w+");

		if (roomFile == NULL)
		{
			perror("Error creating room file.");
			exit(1);
		}

		initialiseRoomFile(roomFile);
	}

	extractRooms(roomFile);
	fclose(roomFile);
}