#include "demo/engine.h"
#include "drivers/keyboard.h"
#include "drivers/vga.h"
#include "core/timer.h"
void engine_init(engine_game_t *g){
    g->running = 1;
}
int engine_running(engine_game_t *g){
    return g->running;
}
void engine_quit(engine_game_t *g){
    g->running = 0;
}
int engine_key(void){
    return _keyboardget();
}
void engine_clear(uint8_t color){
    _clean();
}
void engine_putc(int x, int y, char c, uint8_t color){
    _avgachar(x, y, c, color);
}
int engine_width(void){
    return 80;
}
int engine_height(void){
    return 25;
}
void engine_sleep(uint64_t ticks){
    _timesleep(ticks);
}