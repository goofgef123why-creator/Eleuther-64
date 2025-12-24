#include "drivers/command.h"
#include "drivers/vga.h"
#include "misc/menu.h"
#include "misc/animation.h"
#include "lib/str.h"
#include "drivers/serial.h"
#include "core/scheduler.h"
#include "drivers/keyboard.h"
#include "lib/itoa.h"
#include "core/crash.h"
#include "core/timer.h"
#include "demo/snake.h"
#include "demo/pong.h"
typedef struct {
    const char *name;
    command_fn_t fn;
} command_t;
extern volatile uint64_t timer_ticks;
static command_t commands[] = {
    { "help",  cmd_help  },
    { "clean", cmd_clean },
    { "menu",  cmd_menu  },
    { "anim",  cmd_anim  },
    { "panic", cmd_panic },
    { "snake", cmd_snake },
    { "whoami", cmd_who  },
    { "ltick", cmd_tick},
    { "end", cmd_end  },
    { "rtick", cmd_rtick},
    { "pong", cmd_pong}
};
void cmd_end(void){
    _killcurrent();
    _svgawrite("\nCURRENT TASK SLEEPED",0x0F);
}
void cmd_pong(void){
    cheappong();
}
void cmd_rtick(void){
    timer_ticks = 0;
}
void cmd_tick(void){
    char *buf[16];
    itoa(timer_ticks, *buf, 10);
    _svgawrite("\n",0x0F);
    _svgawrite(*buf,0x0F);
}
void cmd_who(void){
    _svgawrite("\nLIVE USER",0x0F);
}
void cmd_help(void) {
    _vgawrite("\nhelp, ltick, clean, menu, anim, panic, rtick, whoami, snake, end, pong", 0x0F);
}
void cmd_clean(void) {
    _clean();
}
void cmd_menu(void) {
    _clean();
    menu(0);
}
void cmd_anim(void){
    animation();
}
void cmd_panic(void){
    _panic("User inflicted error");
}
void cmd_snake(void){
    snake();
}
void cmd_run(const char *input) {
    for (unsigned int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        if (streq(input, commands[i].name)) {
            commands[i].fn();
            return;
        }
    }
}