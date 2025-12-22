#include <stdint.h>
__attribute__((section(".text.kentry")))
#include "drivers/vga.h"
#include "drivers/serial.h"
#include "core/crash.h"
#include "arch/idt.h"
#include "core/timer.h"
#include "arch/pic.h"
#include "misc/menu.h"
#include "drivers/keyboard.h"
#include "core/scheduler.h"
#include "lib/itoa.h"
#define WOB 0x0F
#define GOB 0x08
#define LGOB 0x07
/*thinking for now we should just have a
list of integers/hexs for errors codes
and we map them to strings.
*/
#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))
void _enableinit(){
    _initidt();
    _picremap();
    _picenableirq(0);
    _picenableirq(1);
}
void _continueinit(){
    _inittimer(100);
    asm volatile ("sti");
}
void _kentry(void) {
    _enableinit();
    _continueinit();
    _timesleep(100);
    menu(1);
    shell_loop();
    for (;;){
        __asm__ volatile ("hlt");
    }
}