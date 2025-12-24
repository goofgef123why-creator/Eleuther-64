#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
typedef struct {
    int running;
} engine_game_t;
void engine_init(engine_game_t *g);
int engine_running(engine_game_t *g);
void engine_quit(engine_game_t *g);
int engine_key(void);
void engine_clear(uint8_t color);
void engine_putc(int x, int y, char c, uint8_t color);
int engine_width(void);
int engine_height(void);
void engine_sleep(uint64_t ticks);
#endif
