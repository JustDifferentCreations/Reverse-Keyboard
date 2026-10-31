#include "i2c.h"
#include <avr/io.h>

void I2C_init(void) {
    TWI0.MBAUD = (uint8_t) (((12000000 / (2 * 400000UL)) - 10));
    TWI0.MCTRLA = TWI_ENABLE_bm;  
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

void I2C_start(uint8_t addr) {
    TWI0.MADDR = (addr << 1);
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
}

void I2C_write(uint8_t data) {
    TWI0.MDATA = data;
    while (!(TWI0.MSTATUS & TWI_WIF_bm));
}

void I2C_stop(void) {
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;
}
