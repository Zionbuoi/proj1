#ifndef CENTRE_PRINTSTR_H
#define CENTRE_PRINTSTR_H

#include "mbed.h"
#include "N5110.h"

// Function prototype for centering a string on the LCD
void centerPrintString(N5110 &lcd, const char* str, int row);
int my_strlen(const char* str);

#endif // DISPLAY_UTILS_H