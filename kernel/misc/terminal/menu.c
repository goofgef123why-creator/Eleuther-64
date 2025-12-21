#define WOB 0x0F
#define GOB 0x08
#define LGOB 0x07
#define VERSION "0.1"
#define BIT "64"
#define MODE "Long Mode"
#include "drivers/vga.h"
void menu(int boot){
    _clean();
    _svgawrite("  ===========\n",LGOB);
    _svgawrite("  =*=========\n",LGOB);
    _svgawrite(" ============\n",LGOB);
    _svgawrite("=============\n",LGOB);
    _svgawrite("  ===========\n",LGOB);
    _svgawrite("  ===========\n",LGOB);
    _svgawrite(" ============\n",LGOB);
    _svgawrite("\n",LGOB);
    _svgawrite("ELEUTHER VERSION: ",WOB);
    _svgawrite(VERSION,WOB);
    _svgawrite("\n",WOB);
    _svgawrite(BIT,WOB);
    _svgawrite(" Bit ",WOB);
    _svgawrite(MODE, WOB);
    if (boot){
        _svgawrite("\n",WOB);
    }
    // this is gonna be hard to rewrite 
}