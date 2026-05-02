#ifndef GAME_H
#define GAME_H

void help_text(void);
void interpret_input(void);
void physical_challenge(void);
void puzzle_challenge(void);
void clear_challenge(void);
void challenge_logic(void);
void move_logic(size_t next_room);
void game_logic(void);

#endif
