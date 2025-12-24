#ifndef CMD
#define CMD
typedef void (*command_fn_t)(void);
void cmd_who(void);
void cmd_tick(void);
void cmd_end(void);
void cmd_help(void);
void cmd_clean(void);
void cmd_menu(void);
void cmd_run(const char *input);
void cmd_anim(void);
void cmd_test(void);
void cmd_panic(void);
void cmd_rtick(void);
void cmd_snake(void);
void cmd_pong(void);
#endif