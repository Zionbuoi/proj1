#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {
    const int SCREEN_WIDTH = 84;        // LCD width in pixels
    const int SCREEN_HEIGHT = 48;       // LCD height in pixels
    const int PLAYER_WIDTH = 5;         // Ship sprite width
    const int PLAYER_HEIGHT = 5;        // Ship sprite height
    const int BOTTOM_MARGIN = 1;        // Space from screen bottom
}

namespace AlienConfig {
    const int ALIEN_ROWS = 3;         // Number of alien rows
    const int ALIEN_COLS = 5;         // Number of alien columns
    const int ALIEN_START_X = 10;     // Initial X position (left padding)
    const int ALIEN_START_Y = 10;     // Initial Y position (top padding)
    const int ALIEN_SPACING_X = 12;   // Horizontal space between aliens
    const int ALIEN_SPACING_Y = 8;    // Vertical space between rows
}

#endif