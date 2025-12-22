#include "core/scheduler.h"
#include "core/crash.h"
#include <stdint.h>
extern void _switchto(context_t *prev, context_t *next);
static volatile task_t *current = 0;
#define TIME_SLICE 5
void _schedulerinit(task_t *first) {
    current = first;
    first->ticks = TIME_SLICE;
}
void _schedulertick(void) {
    if (!current){
        return;
    }
    current->ticks--;
    if (current->ticks > 0){
        return;
    }
    current->ticks = TIME_SLICE;
    _schedule();
}
void taskentry(void) {
    for (;;);
}
void _inittask(task_t *t, void *stack, unsigned int size) {
    uint64_t sp = ((uint64_t)stack + size) & ~0xF;
    t->ctx.rsp = sp;
    t->ctx.rip = (uint64_t)taskentry;
    t->ticks = TIME_SLICE;
    t->state = TASK_RUNNING;
}
void yield(void) {
    asm volatile ("int $32"); 
}
void _schedule(void) {
    if (!current){
        _panic("Scheduler: current is NULL");
    }
    if (!current->next){
        _panic("scheduler: task list corrupted");
    }
    task_t *prev = current;
    task_t *next = current->next;
    int scanned = 0;
    while (next->state != TASK_RUNNING) {
        next = next->next;
        scanned++;
        if (!next || scanned > 64){
            _panic("scheduler: no runnable tasks");
        }
    }
    if (next == prev){
        return;
    }
    current = next;
    _switchto(&prev->ctx, &next->ctx);
}
void _schedulerstart(void) {
    if (!current)
        _panic("schedulerstart: no current task");

    asm volatile (
        "mov %0, %%rsp\n"
        "jmp *%1\n"
        :
        : "r"(current->ctx.rsp),
          "r"(current->ctx.rip)
        : "memory"
    );
}