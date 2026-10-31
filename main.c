#include "mcc_generated_files/system/system.h"
#include "usb_core.h"
#include <usb_hid_keycodes.h>
#include "usb_hid_keyboard.h"
#include "util/delay.h"
#include <avr/io.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "i2c.h"
#include "ssd1309.h"
#include "input.h"
#include "ascii_map.h"

int main(void){
    // start system
    SYSTEM_Initialize();
    
    // set interrupt functions
    STORE_1_SetInterruptHandler(STORE_1_InterruptHandler);
    CLEAR_1_SetInterruptHandler(CLEAR_1_InterruptHandler);
    STORE_2_SetInterruptHandler(STORE_2_InterruptHandler);
    CLEAR_2_SetInterruptHandler(CLEAR_2_InterruptHandler);
    STORE_3_SetInterruptHandler(STORE_3_InterruptHandler);
    CLEAR_3_SetInterruptHandler(CLEAR_3_InterruptHandler);
    SW1_SetInterruptHandler(SW1_8_InterruptHandler);
    SW2_SetInterruptHandler(SW1_8_InterruptHandler);
    SW3_SetInterruptHandler(SW1_8_InterruptHandler);
    SW4_SetInterruptHandler(SW1_8_InterruptHandler);
    SW5_SetInterruptHandler(SW1_8_InterruptHandler);
    SW6_SetInterruptHandler(SW1_8_InterruptHandler);
    SW7_SetInterruptHandler(SW1_8_InterruptHandler);
    SW8_SetInterruptHandler(SW1_8_InterruptHandler);
    SW17_SetInterruptHandler(SW17_InterruptHandler);
    SW16_SetInterruptHandler(SW16_InterruptHandler);
    SW15_SetInterruptHandler(SW15_InterruptHandler);
    SEND_SetInterruptHandler(SEND_InterruptHandler);
    
    // start USB connection
    USB_Start();
    
    // turn on storage active toggles
    storage_1_active = (PORTA.IN & (1 << 0)) != 0;
    storage_2_active = (PORTF.IN & (1 << 1)) != 0;
    storage_3_active = (PORTF.IN & (1 << 0)) != 0;
    
    //start screen
    I2C_init();
    SSD1309_init();
    SSD1309_clear();   // clear display first
    current_input_update();
    
    // keep usb connection going
    while(1){
        USBDevice_Handle();
    }    
}

