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
typedef struct {
    const char *name;
    command_fn_t fn;
} command_t;
static task_t shell_task;
static uint8_t shell_stack[4096];
static command_t commands[] = {
    { "help",  cmd_help  },
    { "clean", cmd_clean },
    { "menu",  cmd_menu  },
    { "anim",  cmd_anim  },
    { "scheduletest", cmd_test },
    { "panic", cmd_panic}
};
void _initshelltask(task_t *t) {
    _inittask(t, shell_stack, sizeof(shell_stack));
    t->ctx.rip = (uint64_t)shell_loop;
    t->state = TASK_RUNNING;
}
void cmd_help(void) {
    _vgawrite("\nhelp, clean, menu, anim, scheduletest, panic", 0x0F);
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
static task_t task1, task2;
static uint8_t stack1[4096];
static uint8_t stack2[4096];
volatile uint64_t t1_ticks = 0;
volatile uint64_t t2_ticks = 0;
void task1_entry(void) {
    for (;;) {
        t1_ticks++;
        yield();
    }
}
void task2_entry(void) {
    for (;;) {
        t2_ticks++;
        _timesleep(0.00001);
        if (t2_ticks % 100 == 0) {
            char buf[20];
            itoa(t1_ticks, buf, 10);
            _svgawrite("\nt1=", 0x0F);
            _svgawrite(buf, 0x0F);
            itoa(t2_ticks, buf, 10);
            _svgawrite(" t2=", 0x0F);
            _svgawrite(buf, 0x0F);
            _svgawrite("\n", 0x0F);
        }
        if (t2_ticks > 1000){
            task2.state = TASK_SLEEPING;
            _svgawrite("\n>",0x0F);
        }
        yield();
    }
}
void scheduletest(void) {
    _initshelltask(&shell_task);
    _inittask(&task1, stack1, sizeof(stack1));
    task1.ctx.rip = (uint64_t)task1_entry;
    _inittask(&task2, stack2, sizeof(stack2));
    task2.ctx.rip = (uint64_t)task2_entry;
    shell_task.next = &task1;
    task1.next = &task2;
    task2.next = &shell_task;

    _schedulerinit(&shell_task);
}
void cmd_test(void){
    _clean();
    scheduletest();
    _schedulerstart();
    shell_loop();
}
void cmd_panic(void){
    _panic("User inflicted error");
}
void cmd_run(const char *input) {
    for (unsigned int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        if (_streq(input, commands[i].name)) {
            commands[i].fn();
            return;
        }
    }
}