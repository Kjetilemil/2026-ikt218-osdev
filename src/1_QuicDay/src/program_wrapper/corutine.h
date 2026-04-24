#include "libc/stdint.h";

typedef struct {
    void* task_fn; // Pointer to the functiuon
    int (*init)(int* argc, char* argv);
    int (*resume)();
    int (*pause)();
    int (*terminate)();
    void* context; // Pointer to the context of the corutine (e.g., stack, registers)
    volatile int state; // 0 = running, 1 = terminated, 2 = paused
} Corutine;

Corutine* create_corutine(void* task_fn);

int init_corutine(int* argc, char* argv);
int resume_corutine();
int pause_corutine();
int terminate_corutine();
int save_context(void* context);