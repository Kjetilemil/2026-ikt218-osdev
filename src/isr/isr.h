#ifndef ISR_H
#define ISR_H

#ifdef __cplusplus
extern "C" {
#endif
//setter opp idt, registrerer isr-funksjonen og konfigurerer cpu-interrupts

void isr_initialize(void);

//kalt fra assembly

void isr0(void);
void isr1(void);
void isr14(void);
//funksjonene som håndterer errors

void isr0_handler(void);
void isr1_handler(void);
void isr14_handler(void);

#ifdef __cplusplus
}
#endif


#endif