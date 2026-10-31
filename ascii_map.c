#include "ascii_map.h"
#include <avr/pgmspace.h>
#include <string.h>
#include <usb_hid_keycodes.h>

// Control codes 0..31 plus DEL (127)
const char control_names[33][4] PROGMEM = {
    "NUL","SOH","STX","ETX","EOT","ENQ","ACK","BEL",
    "BS","HT","LF","VT","FF","CR","SO","SI",
    "DLE","DC1","DC2","DC3","DC4","NAK","SYN","ETB",
    "CAN","EM","SUB","ESC","FS","GS","RS","US",
    "DEL"
};

void getAsciiName(uint8_t code, char *buf) {
    const char *src;
    uint8_t i = 0;

    if (code < 32) {
        src = control_names[code];
    } else if (code == 127) {
        src = control_names[32];    // DEL
    } else if (code == 32) {
        src = "SP";                 // space
    } else {
        buf[0] = (char)code;        // printable ASCII
        buf[1] = '\0';
        return;
    }

    // Copy up to 3 chars + terminator from PROGMEM
    char c;
    while ((c = pgm_read_byte(&src[i])) != 0 && i < 3) {
        buf[i] = c;
        i++;
    }
    buf[i] = '\0';
}

int ascii_2_hid_conv(char input) {
    int hid = HID_KEY_NONE; // default if character not found

    switch (input) {
        // Letters
        case 'A': hid = 0xBC; break;
        case 'B': hid = 0xBD; break;
        case 'C': hid = 0xBE; break;
        case 'D': hid = 0xBF; break;
        case 'E': hid = 0xC0; break;
        case 'F': hid = HID_F; break;
        case 'G': hid = HID_G; break;
        case 'H': hid = HID_H; break;
        case 'I': hid = HID_I; break;
        case 'J': hid = HID_J; break;
        case 'K': hid = HID_K; break;
        case 'L': hid = HID_L; break;
        case 'M': hid = HID_M; break;
        case 'N': hid = HID_N; break;
        case 'O': hid = HID_O; break;
        case 'P': hid = HID_P; break;
        case 'Q': hid = HID_Q; break;
        case 'R': hid = HID_R; break;
        case 'S': hid = HID_S; break;
        case 'T': hid = HID_T; break;
        case 'U': hid = HID_U; break;
        case 'V': hid = HID_V; break;
        case 'W': hid = HID_W; break;
        case 'X': hid = HID_X; break;
        case 'Y': hid = HID_Y; break;
        case 'Z': hid = HID_Z; break;
        case 'a': hid = HID_A; break;
        case 'b': hid = HID_B; break;
        case 'c': hid = HID_C; break;
        case 'd': hid = HID_D; break;
        case 'e': hid = HID_E; break;
        case 'f': hid = HID_F; break;
        case 'g': hid = HID_G; break;
        case 'h': hid = HID_H; break;
        case 'i': hid = HID_I; break;
        case 'j': hid = HID_J; break;
        case 'k': hid = HID_K; break;
        case 'l': hid = HID_L; break;
        case 'm': hid = HID_M; break;
        case 'n': hid = HID_N; break;
        case 'o': hid = HID_O; break;
        case 'p': hid = HID_P; break;
        case 'q': hid = HID_Q; break;
        case 'r': hid = HID_R; break;
        case 's': hid = HID_S; break;
        case 't': hid = HID_T; break;
        case 'u': hid = HID_U; break;
        case 'v': hid = HID_V; break;
        case 'w': hid = HID_W; break;
        case 'x': hid = HID_X; break;
        case 'y': hid = HID_Y; break;
        case 'z': hid = HID_Z; break;

        // Numbers
        case '0': hid = HID_0; break;
        case '1': hid = HID_1; break;
        case '2': hid = HID_2; break;
        case '3': hid = HID_3; break;
        case '4': hid = HID_4; break;
        case '5': hid = HID_5; break;
        case '6': hid = HID_6; break;
        case '7': hid = HID_7; break;
        case '8': hid = HID_8; break;
        case '9': hid = HID_9; break;

        // Symbols (as you had them)
        case ' ': hid = HID_SPACEBAR; break;
        case '\n': hid = HID_RETURN; break;
        case '\b': hid = HID_BACKSPACE; break;
        case '\t': hid = HID_TAB; break;
        case '-': hid = HID_UNDERSCORE; break;
        case '=': hid = HID_EQUAL; break;
        case '[': hid = HID_OPEN_BRACE; break;
        case ']': hid = HID_CLOSE_BRACE; break;
        case '\\': hid = HID_BACKSLASH; break;
        case ';': hid = HID_SEMICOLON; break;
        case ':': hid = HID_KEYPAD_COLON; break;
        case '\'': hid = HID_APOSTROPHE; break;
        case '`': hid = HID_GRAVE_TILDE; break;
        case ',': hid = HID_COMMA; break;
        case '.': hid = HID_DOT; break;
        case '/': hid = HID_SLASH; break;
        case '*': hid = HID_KEYPAD_ASTERISK; break;
        case '+': hid = HID_KEYPAD_PLUS; break;
        case '!': hid = HID_KEYPAD_EXCLAMATION; break;
        case '<': hid = HID_KEYPAD_LEFT_ANGLE_BRACE; break;
        case '>': hid = HID_KEYPAD_RIGHT_ANGLE_BRACE; break;
        case '%': hid = HID_KEYPAD_PERCENT; break;
        case '(': hid = HID_KEYPAD_LEFT_PARENTHESIS; break;
        case ')': hid = HID_KEYPAD_RIGHT_PARENTHESIS; break;
    }
    return hid;
}
