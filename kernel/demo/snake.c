#include "demo/engine.h"
#include "core/timer.h"
#include "lib/itoa.h"
#include "drivers/vga.h"
#define maxbody 128
#define up    0
#define down  1
#define left  2
#define right 3
typedef struct{
    int x;
    int y;
} point_t;
uint16_t score = 0;
extern volatile uint64_t timer_ticks;
static point_t snakebody[maxbody];
static int snakebodylen;
static int dir;
static point_t food;
static int foodonsnake(int x, int y){
    for(int i = 0; i < snakebodylen; i++){
        if(snakebody[i].x == x && snakebody[i].y == y){
            return 1;
        }
    }
    return 0;
}
static void spawnfood(void){
    do{
        food.x = timer_ticks % engine_width();
        food.y = (timer_ticks / 7) % engine_height();
    }while (foodonsnake(food.x, food.y));
}
static void draw(void){
    engine_clear(0x00);
    for (int i = 0; i < snakebodylen; i++){
        engine_putc(snakebody[i].x, snakebody[i].y, 'O', 0x0A);
    }
    char buf[16];
    itoa(score, buf, 10);
    _vgawrite(buf, 0x0F);
    engine_putc(food.x, food.y, '*', 0x0C);
}
static void update(void){
    for(int i = snakebodylen - 1; i > 0; i--){
        snakebody[i] = snakebody[i - 1];
    }
    if(dir == up){
        snakebody[0].y--;
    }  
    if(dir == down){
        snakebody[0].y++;
    }
    if(dir == left){
        snakebody[0].x--;
    }
    if(dir == right){
        snakebody[0].x++;
    }
    if(snakebody[0].x == food.x && snakebody[0].y == food.y){
        if(snakebodylen < maxbody){
            snakebodylen += 1;
            score += 1;
        }
        spawnfood();
    }
}
static void input(engine_game_t *g){
    int k = engine_key();
    if(k == 'w' && dir != down){
        dir = up;
    }
    if(k == 's' && dir != up){
        dir = down;
    }
    if(k == 'a' && dir != right){
        dir = left;
    }
    if(k == 'd' && dir != left){
        dir = right;
    }  
    if(k == 'q'){
        score = 0;
        engine_quit(g);
    }
}
static int collision(void){
    if(snakebody[0].x < 0 || snakebody[0].y < 0){
        return 1;
    }
    if(snakebody[0].x >= engine_width() || snakebody[0].y >= engine_height()){
        return 1;
    }
    for(int i = 1; i < snakebodylen; i++){
        if(snakebody[0].x == snakebody[i].x && snakebody[0].y == snakebody[i].y){
            return 1;
        }
    }
    return 0;
}
void snake(void){
    engine_game_t game;
    engine_init(&game);
    snakebodylen = 5;
    dir = right;
    int sx = engine_width() / 2;
    int sy = engine_height() / 2;
    for(int i = 0; i < snakebodylen; i++){
        snakebody[i].x = sx - i;
        snakebody[i].y = sy;
    }
    spawnfood();
    while(engine_running(&game)){
        input(&game);
        update();
        draw();
        if(collision()){
            score = 0;
            engine_quit(&game);
        }
        engine_sleep(10);
    }
    engine_clear(0x00);
}