#include "coroutine_scheduler.h"
#include "coroutine.h"
#include "libc/stddef.h";
#include "libc/stdio.h";

#define MAX_COROUTINES 65536    // Support up to 65536 coroutines per scheduler

void coroutine_scheduler_add(Coroutine_Scheduler* self, Coroutine* co) {
    if (self->coroutine_count >= self->max_coroutines) {
        printf("Error: Coroutine scheduler is full. Cannot add more coroutines.\n");
        return;
    }
    self->coroutine_array[self->coroutine_count++] = co;
}

void coroutine_scheduler_init(Coroutine_Scheduler* self) {

    self->coroutine_array = (Coroutine**)malloc(sizeof(Coroutine) * MAX_COROUTINES);

    while (1) {
        for (int i = 0; i < self->coroutine_count; i++) {
            Coroutine* co = self->coroutine_array[i];

            switch (co->state) {
                case CO_NEW:
                    co->init(co, 0, NULL);
                    break;

                case CO_RUNNING:
                    co->run(co);
                    break;

                case CO_PAUSED:
                    // skip
                    break;

                case CO_TERMINATED:
                    free(co->context);
                    free(co);

                    // remove from list
                    self->coroutine_array[i] = self->coroutine_array[--self->coroutine_count];
                    i--;
                    break;
            }
        }
    }
}
