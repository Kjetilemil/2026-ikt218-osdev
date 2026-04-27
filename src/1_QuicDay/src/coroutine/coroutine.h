#ifndef COROUTINE_H
#define COROUTINE_H

#include "libc/stdint.h";

typedef struct {
    void (*task_fn)(struct Coroutine* self); // Pointer to the actual coroutine function
    int (*init)(struct Coroutine* self, int argc, char** argv);
    int (*run)(struct Coroutine* self);
    int (*resume)(struct Coroutine* self);
    int (*pause)(struct Coroutine* self);
    int (*terminate)(struct Coroutine* self);
    void* context; // Pointer to the context of the corutine (e.g., stack, registers)
    volatile int state; // 0 = NEW, 1 = RUNNING, 2 = PAUSED, 3 = TERMINATED
} Coroutine;

Coroutine* create_corutine(void (*task_fn)(Coroutine*));

int init_coroutine(Coroutine* self, int argc, char** argv);
int run_coroutine(Coroutine* self);
int resume_coroutine(Coroutine* sealf);
int pause_coroutine(Coroutine* self);
int terminate_coroutine(Coroutine* self);

#endif // COROUTINE_H