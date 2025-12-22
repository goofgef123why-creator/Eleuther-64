#ifndef CMD
#define CMD
typedef void (*command_fn_t)(void);
void cmd_help(void);
void cmd_clean(void);
void cmd_menu(void);
void cmd_run(const char *input);
void cmd_anim(void);
void cmd_test(void);
void cmd_panic(void);
#endif