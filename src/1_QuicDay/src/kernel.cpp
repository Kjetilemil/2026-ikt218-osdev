extern "C" void isr_initialize(void);
extern "C" int kernel_main(void);

int kernel_main() {
    isr_initialize();

    asm volatile("int $0x0");
    asm volatile("int $0x1");
    asm volatile("int $0xE");

    while (1) {
    }

    return 0;
}