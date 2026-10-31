#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C_init(void);
void I2C_start(uint8_t addr);
void I2C_write(uint8_t data);
void I2C_stop(void);

#endif // I2C_H
