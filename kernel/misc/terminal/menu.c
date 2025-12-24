#define WOB 0x0F
#define GOB 0x08
#define LGOB 0x07
#define VERSION "0.2"
#define BIT "64"
#define MODE "Long Mode"
#include "drivers/vga.h"
#include "drivers/command.h"
#include "drivers/keyboard.h"
void menu(int boot){
    if (!boot){
        _clean();
    }
    _svgawrite("Eleuther 64 v", WOB);
    _svgawrite(VERSION, WOB);
    _svgawrite("\n", WOB);
    _svgawrite("CPU: ", WOB);
    _svgawrite(BIT, WOB);
    _svgawrite(" Bit ", WOB);
    _svgawrite(MODE, WOB);
    _svgawrite("\n", WOB);
    if (boot){
        _svgawrite("\n", WOB);
        _svgawrite(">",WOB);
        _shellloop();
    }
}