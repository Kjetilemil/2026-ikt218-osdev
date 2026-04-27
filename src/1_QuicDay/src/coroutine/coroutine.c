#include "coroutine.h";
#include "libc/stddef.h"
#include "libc/stdio.h"
#include "kernel_memory.h"
#include "pit.h"

Coroutine* create_corutine(void (*task_fn)(Coroutine*)) {
    Coroutine* routine = malloc(sizeof(Coroutine));

    if (!routine) {
        printf("Failed to allocate memory for coroutine.\n");
        sleep_interrupt(2000); // Sleep for 2000 ms to allow the message to be seen
        return NULL;
    } // Allocation failed

    routine->task_fn = task_fn;
    routine->init = init_coroutine;
    routine->resume = resume_coroutine;
    routine->pause = pause_coroutine;
    routine->terminate = terminate_coroutine;
    routine->context = NULL; // Context will be set during initialization by the developer
    routine->state = 0; // NEW

    return routine;
}

int init_coroutine(Coroutine* self, int argc, char** argv) {
    if (self->state != 0) return -1; // must be NEW
    if (!self->task_fn) return -1; // No task function assigned

    self->state = 1; // Set state to RUNNING
    return 0; // Return 0 on success
}

int run_coroutine(Coroutine* self) {
    if (self->state != 1) return -1; // must be RUNNING

    self->task_fn; // Call the task function with the coroutine as an argument
    return 0; // Return 0 on success
}

int resume_coroutine(Coroutine* self) {
    if (self->state != 2) return -1; // must be PAUSED

    self->state = 1; // Set state to RUNNING
    return 0; // Return 0 on success
}

int pause_coroutine(Coroutine* self) {
    if (self->state != 1) return -1; // must be RUNNING

    self->state = 2; // Set state to PAUSED
    return 0; // Return 0 on success
}

int terminate_coroutine(Coroutine* self) {
    if (self->state == 3) return -1; // already TERMINATED

    self->state = 3; // Set state to TERMINATED
    return 0; // Return 0 on success
}