#ifndef SSD1309_H
#define SSD1309_H

#include <stdint.h>

void SSD1309_init(void);
void SSD1309_command(uint8_t cmd);
void SSD1309_data(uint8_t *data, uint16_t len);

void SSD1309_setCursor(uint8_t page, uint8_t col);
void SSD1309_clear(void);
void SSD1309_clearRow2x(uint8_t row);
void SSD1309_clearChar2x(uint8_t row, uint8_t col);
void SSD1309_drawChar2x(uint8_t page, uint8_t col, char c);
void SSD1309_print2x(uint8_t row, uint8_t col, const char *str);

#endif // SSD1309_H
