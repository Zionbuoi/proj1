#include "mainplayer.h"
#include "Joystick.h"
#include "N5110.h"
#include "constants.h"
#include "mbed.h"

Player::Player(int x, int y, int width, int height, int *data)
    : _x(x), _y(y), _width(width), _height(height), _data(data) {}

// Move function used to interface the player to the joystick
void Player::move(N5110 &lcd, Joystick &joystick) {
    Vector2D coord = joystick.get_mapped_coord();
    
    const float deadzone = 0.2f;
    const int sensitivity = 3;  // Movement is fixed at 3 px/second to make player speeds a bit more managable.
    const int screen_right = Constants::SCREEN_WIDTH - _width;
    
    if (coord.x < -deadzone && _x > 0) {
        _x -= sensitivity;
        if (_x < 0) _x = 0;
    }

    else if (coord.x > deadzone && _x < screen_right) {
        _x += sensitivity;
        if (_x > screen_right) _x = screen_right;
    }

    _y = Constants::SCREEN_HEIGHT - _height - Constants::BOTTOM_MARGIN;
}