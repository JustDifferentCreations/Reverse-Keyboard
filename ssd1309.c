#include "ssd1309.h"
#include "i2c.h"
#include "font5x7.h"
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>

#define SSD1309_I2C_ADDR  0x3C  // typical 0x3C or 0x3D depending on panel

void SSD1309_command(uint8_t cmd) {
    I2C_start(SSD1309_I2C_ADDR);
    I2C_write(0x00);   // Control byte: Co=0, D/C#=0
    I2C_write(cmd);
    I2C_stop();
}

void SSD1309_data(uint8_t *data, uint16_t len) {
    I2C_start(SSD1309_I2C_ADDR);
    I2C_write(0x40);   // Control byte: Co=0, D/C#=1
    for (uint16_t i=0; i<len; i++) {
        I2C_write(data[i]);
    }
    I2C_stop();
}

void SSD1309_init(void) {
    _delay_ms(100);
    SSD1309_command(0xAE); // Display off
    SSD1309_command(0x20); // Memory addressing mode
    SSD1309_command(0x00); // Horizontal addressing
    SSD1309_command(0xB0); // Page start address
    SSD1309_command(0xC8); // COM scan direction remapped
    SSD1309_command(0x00); // Low column
    SSD1309_command(0x10); // High column
    SSD1309_command(0x40); // Start line
    SSD1309_command(0x81); SSD1309_command(0x7F); // Contrast
    SSD1309_command(0xA1); // Segment remap
    SSD1309_command(0xA6); // Normal display
    SSD1309_command(0xA8); SSD1309_command(0x3F); // Multiplex ratio
    SSD1309_command(0xA4); // Output follows RAM
    SSD1309_command(0xD3); SSD1309_command(0x00); // Display offset
    SSD1309_command(0xD5); SSD1309_command(0x80); // Display clock
    SSD1309_command(0xD9); SSD1309_command(0xF1); // Precharge
    SSD1309_command(0xDA); SSD1309_command(0x12); // COM pins
    SSD1309_command(0xDB); SSD1309_command(0x40); // VCOM detect
    SSD1309_command(0x8D); SSD1309_command(0x14); // Enable charge pump
    SSD1309_command(0xAF); // Display on
}

const uint8_t font5x7_local_dummy[1] PROGMEM = {0}; // keep file consistent if header usage

void SSD1309_setCursor(uint8_t page, uint8_t col) {
    SSD1309_command(0xB0 + page);
    SSD1309_command(0x00 + (col & 0x0F));
    SSD1309_command(0x10 + ((col >> 4) & 0x0F));
}

void SSD1309_drawChar2x(uint8_t page, uint8_t col, char c) {
    const uint8_t *bitmap = font5x7[c - 0x20];
 
    // Each page is 8px tall. A 16px tall char = 2 pages high.
    for (uint8_t yblock = 0; yblock < 2; yblock++) {
        SSD1309_setCursor(page + yblock, col);
        uint8_t out[12];  // scaled character = 12 columns wide
        uint8_t idx = 0;

        for (uint8_t i = 0; i < 6; i++) {
            uint8_t colbits = pgm_read_byte(&bitmap[i]);  // original 8px tall column
            uint16_t doubled = 0;
 
            // Scale vertically: expand each bit into 2 bits
            for (uint8_t bit = 0; bit < 8; bit++) {
                if (colbits & (1 << bit)) {
                    doubled |= (3 << (bit * 2)); // make 2px tall block
                }
            }

            // Pick correct half (top 8 rows or bottom 8 rows)
            uint8_t upper = doubled & 0xFF;
            uint8_t lower = (doubled >> 8) & 0xFF;

            uint8_t scaled = (yblock == 0) ? upper : lower;

            // Scale horizontally: duplicate each column
            out[idx++] = scaled;
            out[idx++] = scaled;
        }

        SSD1309_data(out, 12);
    }
}

void SSD1309_print2x(uint8_t row, uint8_t col, const char *str) {
    // row = 0..3 (4 lines), col = pixel column (0..127)
    uint8_t page = row * 2;  // each char is 2 pages tall
    while (*str) {
        SSD1309_drawChar2x(page, col, *str++);
        col += 11 ;  // each char is 12px wide (we advance 11 to overlap spacing)
        if (col >= 128) {
            col = 0;
            page += 2;
            if (page >= 8) break; // screen full
        }
    }
}

void SSD1309_clear(void) {
    for (uint8_t page = 0; page < 8; page++) {
        SSD1309_setCursor(page, 0);
        uint8_t zeros[128];
        memset(zeros, 0x00, 128);
        SSD1309_data(zeros, 128);
    }
}

void SSD1309_clearRow2x(uint8_t row) {
    if (row >= 4) return;   // only 0..3 valid
    uint8_t page = row * 2;
    uint8_t zeros[128];
    memset(zeros, 0x00, 128);

    // Clear both pages of this row
    SSD1309_setCursor(page, 0);
    SSD1309_data(zeros, 128);

    SSD1309_setCursor(page + 1, 0);
    SSD1309_data(zeros, 128);
}

void SSD1309_clearChar2x(uint8_t row, uint8_t col) {
    if (row >= 4) return;   // 4 rows max (0..3)
    uint8_t page = row * 2;
    uint8_t x = col * 11;   // char width = 12 px

    uint8_t zeros[11];
    memset(zeros, 0x00, 11);

    // Clear top 8 pixels (first page of char)
    SSD1309_setCursor(page, x);
    SSD1309_data(zeros, 11);

    // Clear bottom 8 pixels (second page of char)
    SSD1309_setCursor(page + 1, x);
    SSD1309_data(zeros, 11);
}
