#ifndef ASCII_MAP_H
#define ASCII_MAP_H

#include <stdint.h>

int ascii_2_hid_conv(char input);
void getAsciiName(uint8_t code, char *buf);

#endif // ASCII_MAP_H
