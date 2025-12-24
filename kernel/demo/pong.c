#include "demo/engine.h"
#include "core/timer.h"
#include "demo/pong.h"
#define PADDLE_WIDTH 7
typedef struct {
    int x;
    int y;
} point_t;

static point_t ball;
static int ball_dx;
static int ball_dy;
static int paddle_x;
static int paddle_y;
static void draw(void){
    engine_clear(0x00);
    for(int i = 0; i < PADDLE_WIDTH; i++){
        engine_putc(paddle_x + i, paddle_y, '#', 0x0F);
    }
    engine_putc(ball.x, ball.y, 'O', 0x0A);
}
static void input(engine_game_t *g){
    int k = engine_key();
    if(k == 'a' && paddle_x > 0){
        paddle_x -= 4;
    }
    if(k == 'd' && paddle_x + PADDLE_WIDTH < engine_width()){
        paddle_x += 4;
    }
    if(k == 'q'){
        engine_quit(g);
    }
}
static void update(engine_game_t *g){
    ball.x += ball_dx;
    ball.y += ball_dy;
    if(ball.x <= 0 || ball.x >= engine_width() - 1){
        ball_dx = -ball_dx;
    }
    if(ball.y <= 0){
        ball_dy = -ball_dy;
    }
    if(ball.y == paddle_y - 1){
        if(ball.x >= paddle_x - 1 && ball.x < paddle_x + PADDLE_WIDTH + 1){
            ball_dy = -ball_dy;
        }
    }
    if(ball.y >= engine_height()){
        engine_quit(g);
    }
}
void cheappong(void){
    engine_game_t game;
    engine_init(&game);
    paddle_y = engine_height() - 2;
    paddle_x = (engine_width() / 2) - (PADDLE_WIDTH / 2);
    ball.x = engine_width() / 2;
    ball.y = engine_height() / 2;
    ball_dx = 1;
    ball_dy = 1;
    while(engine_running(&game)){
        input(&game);
        update(&game);
        draw();
        engine_sleep(15);
    }
    engine_clear(0x00);
}