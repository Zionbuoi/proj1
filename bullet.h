#ifndef BULLET_H
#define BULLET_H

#include "mbed.h"
#include "N5110.h"
#include "mainplayer.h"
#include "sprites.h"

class Bullet {
public:
    Bullet();
    void shoot(int xBullet, int yBullet, int direction, bool check_cooldown = true);
    void update();
    void draw(N5110 &lcd);
    bool isActive() const;
    bool canFire();
    void deactivate();
    int getX() const;
    int getY() const;

private:
    int _xBullet, _yBullet;
    bool _active;
    int _direction;
    Timer cooldown_timer;
    Timer move_timer;
};

#endif