#include "input.h"
#include "ssd1309.h"
#include "ascii_map.h"
#include "usb_hid_keyboard.h"

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

// stored key inputs
int storage_1;
int storage_2;
int storage_3;
bool storage_1_active;
bool storage_2_active;
bool storage_3_active;
int current_input;

void portDToBinaryString(char *str, uint8_t value) {
    for (int i = 7; i >= 0; i--) {
        str[7 - i] = (value & (1 << i)) ? '1' : '0';
    }
    str[8] = '\0'; // null-terminate
}

char safeAscii(uint8_t v) {
    if (v < 0x20 || v > 0x7E) {
        return (char) ' '; // non-printable -> space
    } else {
        return (char) v;
    }
}

void current_input_update(void) {
    uint8_t portState = PORTD.IN;
    char buf[9];
    portDToBinaryString(buf, portState);
    current_input = portState;

    SSD1309_print2x(3, 0, buf);
    SSD1309_clearChar2x(3, 8);
    SSD1309_clearChar2x(3, 9);
    SSD1309_clearChar2x(3, 10);
    char nameBuf[4];
    getAsciiName(portState, nameBuf);
    SSD1309_print2x(3, 127 - ((strlen(nameBuf))*11), nameBuf);
    return;
}

void STORE_1_InterruptHandler(void) {
    storage_1 = current_input;
    SSD1309_clearRow2x(0);
    char buf[9];
    portDToBinaryString(buf, storage_1);
    SSD1309_print2x(0, 0, buf);
    char nameBuf[4];
    getAsciiName(storage_1, nameBuf);
    SSD1309_print2x(0, 127 - ((strlen(nameBuf))*11), nameBuf);
    return;
}

void CLEAR_1_InterruptHandler(void) {
    storage_1 = 0;
    SSD1309_clearRow2x(0); // clear only line 3
    return;
}

void STORE_2_InterruptHandler(void) {
    storage_2 = current_input;
    SSD1309_clearRow2x(1);
    char buf[9];
    portDToBinaryString(buf, storage_2);
    SSD1309_print2x(1, 0, buf);
    char nameBuf[4];
    getAsciiName(storage_2, nameBuf);
    SSD1309_print2x(1, 127 - ((strlen(nameBuf))*11), nameBuf);
    return;
}

void CLEAR_2_InterruptHandler(void) {
    storage_2 = 0;
    SSD1309_clearRow2x(1); // clear only line 3
    return;
}

void STORE_3_InterruptHandler(void) {
    storage_3 = current_input;
    SSD1309_clearRow2x(2);
    char buf[9];
    portDToBinaryString(buf, storage_3);
    SSD1309_print2x(2, 0, buf);
    char nameBuf[4];
    getAsciiName(storage_3, nameBuf);
    SSD1309_print2x(2, 127 - ((strlen(nameBuf))*11), nameBuf);
    return;
}

void CLEAR_3_InterruptHandler(void) {
    storage_3 = 0;
    SSD1309_clearRow2x(2); // clear only line 3
    return;
}

void SW1_8_InterruptHandler(void) {
    current_input_update();
    return;
}

void SW17_InterruptHandler(void) {
    // update when switch toggles
    storage_1_active = (PORTA.IN & (1 << 0)) != 0;
    return;
}

void SW16_InterruptHandler(void) {
    // update when switch toggles
    storage_2_active = (PORTF.IN & (1 << 1)) != 0;
    return;
}

void SW15_InterruptHandler(void) {
    // update when switch toggles
    storage_3_active = (PORTF.IN & (1 << 0)) != 0;
    return;
}

int ascii_2_hid_conv(char input); // forward decl (implemented in ascii_map.c)

void SEND_InterruptHandler(void) {
    cli(); // Deactivate interrupts

    if (storage_1_active) {
        USB_HIDKeyPressDown(ascii_2_hid_conv(safeAscii(storage_1)));
        USB_HIDKeyPressUp(ascii_2_hid_conv(safeAscii(storage_1)));
    }
    if (storage_2_active) {
        USB_HIDKeyPressDown(ascii_2_hid_conv(safeAscii(storage_2)));
        USB_HIDKeyPressUp(ascii_2_hid_conv(safeAscii(storage_2)));
    }
    if (storage_3_active) {
        USB_HIDKeyPressDown(ascii_2_hid_conv(safeAscii(storage_3)));
        USB_HIDKeyPressUp(ascii_2_hid_conv(safeAscii(storage_3)));
    }
    USB_HIDKeyPressDown(ascii_2_hid_conv(safeAscii(current_input)));
    USB_HIDKeyPressUp(ascii_2_hid_conv(safeAscii(current_input)));
    sei(); // Activate interrupt
    return;
}