#include <stdint.h>
__attribute__((section(".text.kentry")))
#include "vga/vga.h"
#include "serial/serial.h"
#include "crash/crash.h"
#include "idt/idt.h"
#include "timer/timer.h"
#include "pic/pic.h"
#define WOB 0x0F
#define GOB 0x08
#define LGOB 0x07
#define VERSION "0.1"
#define BIT "64"
#define MODE "Long Mode"
/*thinking for now we should just have a
list of integers/hexs for errors codes
and we map them to strings.
*/
#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))
void menu(void){
    _clean();
    _vgawrite("  ===========\n",LGOB);
    _vgawrite("  =*=========\n",LGOB);
    _vgawrite(" ============\n",LGOB);
    _vgawrite("=============\n",LGOB);
    _vgawrite("  ===========\n",LGOB);
    _vgawrite("  ===========\n",LGOB);
    _vgawrite(" ============\n",LGOB);
    _vgawrite("\n",LGOB);
    _vgawrite("ELEUTHER VERSION: ",WOB);
    _vgawrite(VERSION,WOB);
    _vgawrite("\n",WOB);
    _vgawrite(BIT,WOB);
    _vgawrite(" Bit ",WOB);
    _vgawrite(MODE, WOB);
    _vgawrite("\n",WOB);
    _vgawrite(">\n",WOB);
    // this is gonna be hard to rewrite 
}
void animation_test(void){
    _clean();
    _vgawrite("INSERT DISK A",WOB);
    _timesleep(50);
    _clean();
    _timesleep(50);
    _vgawrite("INSERT DISK A",WOB);
    _timesleep(50);
}
void _enableinit(){
    _initidt();
    _picremap();
    _picenableirq(0);
}
void _continueinit(){
    _inittimer(100);
    asm volatile ("sti");
}
void _kentry(void) {
    _enableinit();
    _continueinit();
    for (;;){
        animation_test();
    }
    for (;;){
        __asm__ volatile ("hlt");
    }
}