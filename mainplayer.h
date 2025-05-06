#ifndef MAINPLAYER_H
#define MAINPLAYER_H

#include "N5110.h"
#include "mbed.h"
#include "Joystick.h"

class Player {
public:
    Player(int x, int y, int width, int height, int *data);
    void move(N5110 &lcd, Joystick &joystick);
    int getX() const { return _x; }
    int getY() const { return _y; }

private:
    int _x, _y;
    int _width, _height;
    int *_data;
};

#endif