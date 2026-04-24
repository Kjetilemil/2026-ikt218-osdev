#include "corutine.h";
#include "libc/stddef.h"

Corutine* create_corutine(void* task_fn) {
    Corutine* rutine = (Corutine*)malloc(sizeof(Corutine));
    if (!rutine) {
        return NULL; // Allocation failed
    }
    rutine->task_fn = task_fn; // Set to NULL for now, can be set later
    rutine->init = init_corutine;
    rutine->resume = resume_corutine;
    rutine->pause = pause_corutine;
    rutine->terminate = terminate_corutine;
    rutine->context = save_context; // Context will be set when the coroutine is initialized
    rutine->state = 0; // Start in running state
    return rutine;
}