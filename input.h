#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

extern int storage_1;
extern int storage_2;
extern int storage_3;
extern bool storage_1_active;
extern bool storage_2_active;
extern bool storage_3_active;
extern int current_input;

void current_input_update(void);

void STORE_1_InterruptHandler(void);
void CLEAR_1_InterruptHandler(void);
void STORE_2_InterruptHandler(void);
void CLEAR_2_InterruptHandler(void);
void STORE_3_InterruptHandler(void);
void CLEAR_3_InterruptHandler(void);
void SW1_8_InterruptHandler(void);
void SW17_InterruptHandler(void);
void SW16_InterruptHandler(void);
void SW15_InterruptHandler(void);
void SEND_InterruptHandler(void);

void portDToBinaryString(char *str, uint8_t value);
char safeAscii(uint8_t v);

#endif // INPUT_H
