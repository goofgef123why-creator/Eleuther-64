#include "drivers/command.h"
#include "drivers/vga.h"
#include "misc/menu.h"
#include "misc/animation.h"
#include "lib/str.h"
typedef struct {
    const char *name;
    command_fn_t fn;
} command_t;
static command_t commands[] = {
    { "help",  cmd_help  },
    { "clean", cmd_clean },
    { "menu",  cmd_menu  },
    { "anim",  cmd_anim}
};
void cmd_help(void) {
    _vgawrite("\nhelp, clean, menu, anim", 0x0F);
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
void cmd_run(const char *input) {
    for (unsigned int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        if (_streq(input, commands[i].name)) {
            commands[i].fn();
            return;
        }
    }
}