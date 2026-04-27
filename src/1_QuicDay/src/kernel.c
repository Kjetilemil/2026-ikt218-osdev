#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include <multiboot2.h>

#include "vga.h"
#include "../../gdt/gdt.h"
#include "isr.h"
#include "irq.h"
#include "kernel_memory.h"
#include "pit.h"
#include "libc/string.h"

#include "../quicmusic/song.h"
#include "../quicmusic/frequencies.h"

#include "coroutine/coroutine.h"
#include "coroutine/coroutine_scheduler.h"

extern uint32_t end;


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main(void);

enum {
    SCREEN_PAUSE_MS = 4000
};

static void wait_for_user_next_screen(void) {
    uint32_t start_events = irq_get_keyboard_event_count();
    printf("\nPress any key to continue...\n");
    while (irq_get_keyboard_event_count() == start_events) {
        asm volatile("sti; hlt");
    }
}

static void halt_forever(void) {
    while (true) {
        asm volatile("hlt");
    }
}


int compute(int a, int b) { 
    return a + b;
}

// Example to make data from assembly to C 
typedef struct{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e[6];
} MyStruct; 

void task_A(Coroutine* self) {
    while (self->state != 3) { // While not TERMINATED
        printf("Task A is running.\n");
        sleep_interrupt(1000); // Sleep for 1000 ms
        printf("Task A is yielding.\n");
        self->pause(self); // Yield to the scheduler
        
        // When resuming
        printf("Task A has been resumed\n");
    }
    self->terminate(self); // Terminate itself when done
}

void task_B() {
        printf("Task B is running.\n");
                sleep_interrupt(1000); // Sleep for 1000 ms
        printf("Task B Starting coroutine A.\n");
                sleep_interrupt(1000); // Sleep for 1000 ms
        Coroutine* coA = create_corutine(task_A);
        coA->init(coA, 0, NULL);
        coA->run(coA); // Start coroutine A
        printf("Coroutine A paused itself.\n");
                sleep_interrupt(1000); // Sleep for 1000 ms
        printf("Task B Resuming Task A.\n");
                sleep_interrupt(1000); // Sleep for 1000 ms
        coA->resume(coA); // Resume coroutine A
        printf("Task A has been paused again.\n");
        printf("Task B Terminating Task A.\n");
        coA->terminate(coA); // Terminate coroutine A
        free(coA->context); // Free the context of the coroutine
        free(coA); // Free the coroutine after termination
}

int main(uint32_t myStruct, uint32_t magic, struct multiboot_info* mb_info_addr) {

    MyStruct* myStructPtr = (MyStruct*)myStruct; 
    
    int noop = 0; 
    int res = compute(1,2); 

    (void)magic;
    (void)mb_info_addr;
    (void)myStructPtr;
    (void)noop;
    (void)res;

    initGdt(); // Initialize Global Descriptor Table
    isr_initialize();
    irq_initialize();

    init_kernel_memory(&end);
    init_paging();
    init_pit();

    asm volatile("sti");

    print_memory_layout();
    sleep_interrupt(SCREEN_PAUSE_MS);
    /*wait_for_user_next_screen();

    printf("Hello World!\n");
    sleep_interrupt(SCREEN_PAUSE_MS);
    wait_for_user_next_screen();

    void* some_memory = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(13331);

    printf("malloc #1: %p\n", some_memory);
    printf("malloc #2: %p\n", memory2);
    printf("malloc #3: %p\n", memory3);
    sleep_interrupt(SCREEN_PAUSE_MS);
    wait_for_user_next_screen();

    int cpp_status = kernel_main();
    if (cpp_status != 0) {
        printf("kernel_main failed: %d\n", cpp_status);
        halt_forever();
    }
    sleep_interrupt(SCREEN_PAUSE_MS);
    wait_for_user_next_screen();

    uint32_t counter = 0;
    uint32_t start_tick_busy = get_current_tick();
    printf("[%u]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
    sleep_busy(SCREEN_PAUSE_MS);
    uint32_t elapsed_busy = get_current_tick() - start_tick_busy;
    printf("[%u]: Slept using busy-waiting. ticks=%u\n", counter++, elapsed_busy);
    sleep_interrupt(SCREEN_PAUSE_MS);
    wait_for_user_next_screen();

    uint32_t start_tick_interrupt = get_current_tick();
    printf("[%u]: Sleeping with interrupts (LOW CPU).\n", counter);
    sleep_interrupt(SCREEN_PAUSE_MS);
    uint32_t elapsed_interrupt = get_current_tick() - start_tick_interrupt;
    printf("[%u]: Slept using interrupts. ticks=%u\n", counter++, elapsed_interrupt);
    sleep_interrupt(SCREEN_PAUSE_MS);
    wait_for_user_next_screen();

    printf("PIT timing test complete. Playing sound test....\n");

    printf("Enabling speaker\n");
    enable_speaker();

    printf("Playing note A6 for 1000 ms...\n");
    play_sound(A6);
    sleep_interrupt(1000);

    printf("Playing music_1...\n");

    Song* song = malloc(sizeof(Song));
    song->notes = music_1;
    song->length = sizeof(music_1) / sizeof(Note);
    play_song(song);

    printf("Disabling speaker\n");
    disable_speaker();

    halt_forever();

    //Free memory before exiting
    free(some_memory);
    free(memory2);
    free(memory3);*/

    task_B();

    wait_for_user_next_screen();

    halt_forever();

    return 0;
}