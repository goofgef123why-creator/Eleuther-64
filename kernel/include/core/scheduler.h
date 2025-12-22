#pragma once
#include <stdint.h>
typedef struct context {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t rbx;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t rip;
} context_t;
typedef enum {
    TASK_RUNNING,
    TASK_SLEEPING
} task_state_t;
typedef struct task {
    context_t ctx;
    task_state_t state;
    int ticks;
    struct task *next;
} task_t;
void _inittask(task_t *t, void *stack, unsigned int size);
void _schedulertick(void);
void _schedule(void);
void _schedulerinit(task_t *first);
void yield(void);
void _schedulerstart(void);