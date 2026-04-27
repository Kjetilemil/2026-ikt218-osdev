#ifndef COROUTINE_SCHEDULER_H
#define COROUTINE_SCHEDULER_H

#include "coroutine.h"
#include "libc/stdio.h";

typedef enum {
    CO_NEW,
    CO_RUNNING,
    CO_PAUSED,
    CO_TERMINATED
} CoroutineState;

typedef struct Coroutine_Scheduler {
    void (*add)(struct Coroutine_Scheduler* self, Coroutine* co);
    void (*init)(struct Coroutine_Scheduler* self);

    uint16_t max_coroutines;
    uint16_t coroutine_count;
    uint16_t current_index;
    Coroutine** coroutine_array;
} Coroutine_Scheduler;

void coroutine_scheduler_init(Coroutine_Scheduler* self);
void coroutine_scheduler_add(Coroutine_Scheduler* self, Coroutine* co);

#endif // COROUTINE_SCHEDULER_H