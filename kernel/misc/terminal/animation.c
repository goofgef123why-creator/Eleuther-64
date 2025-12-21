#include "drivers/vga.h"
#include "core/timer.h"
#include "misc/animation.h"
void _update(int c){
    if (c == 0){
        _svgawrite("=",0x0F);
    }
    if (c == 1){
        _svgawrite("==",0x0F);
    }
    if (c == 2){
        _svgawrite("=   =\n",0x0F);
        _svgawrite(" ==",0x0F);
    }
    if (c == 3){
        _svgawrite("=     =\n",0x0F);
        _svgawrite(" =   =\n",0x0F);
        _svgawrite("   ==   ",0x0f);
    }
    return;
}
void animation(void){
    for (int i = 0; i < 4; i++){
        _clean();
        _update(i);
        _timesleep(100);
    }
}