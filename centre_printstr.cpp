#include "centre_printstr.h"
#include <cstring>  // Needed for strlen but does work for some reason idk

// Manual implementation of strlen to count number of characters
int my_strlen(const char* str) {
    int len = 0; //character length
    while (str[len] != '\0') { // count all characters till you meet the end
        len++;
    }
    return len;
}

// Function to center text on the LCD
// simply inpu lcd, [insert TEXT], desired row location
void centerPrintString(N5110 &lcd, const char* str, int row) {
    int start_col = (14 - my_strlen(str)) / 2;  // Calulates the starting column
    if (start_col < 0) start_col = 0;  // just to make  sure values are always positive
    lcd.printString(str, start_col, row);
}