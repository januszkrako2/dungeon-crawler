#ifndef FILE_H
#define FILE_H

#include <stdbool.h>

void initialise_room_file(FILE* room_file);
void error_check(struct file* info);
void extract_room_number(struct file* info);
void extract_room_message(struct file* info);
void add_room_connection(struct connection connection, struct file* info, enum direction direction);
void extract_room_connections(struct file* info);
void add_room_challenges(struct file* info, size_t* challenge_counter);
void extract_room_challenges(struct file* info);
void introductory_text_check(struct file info);
void extract_introductory_text(struct file* info);
void update_line(struct file* info);
void extract(FILE* room_file);
void load(void);

#endif
