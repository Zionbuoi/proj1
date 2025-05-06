#include "mbed.h"
#include "N5110.h"
#include "bullet.h"
#include "mainplayer.h"

Bullet::Bullet() : _xBullet(0), _yBullet(0), _active(false), _direction(-1) {
    cooldown_timer.start();
    move_timer.start();
}

bool Bullet::canFire() {
    return !_active && std::chrono::duration_cast<std::chrono::milliseconds>(cooldown_timer.elapsed_time()).count() > 1000;
}

void Bullet::shoot(int xBullet, int yBullet, int direction, bool check_cooldown) {
    if (!check_cooldown || canFire()) {
        _xBullet = xBullet;
        _yBullet = yBullet;
        _direction = direction;
        _active = true;
        if (check_cooldown) cooldown_timer.reset();
    }
}

void Bullet::update() {
    if (_active && move_timer.elapsed_time() > 100ms) {
        _yBullet += _direction;  // Direction controls up/down
        move_timer.reset();

        // Deactivate when out of bounds
        if (_yBullet < 0 || _yBullet >= 48) {
            _active = false;
        }
    }
}

void Bullet::draw(N5110 &lcd) {
    if (_active) {
        if (_direction == -1) { // Player's bullet
            lcd.setPixel(_xBullet, _yBullet);
            lcd.setPixel(_xBullet, _yBullet + 1);
        } else { // Alien's bullet
            lcd.drawSprite(_xBullet, _yBullet, 3, 4, (int*)projectile);
        }
    }
}

void Bullet::deactivate() {
    _active = false;
}

int Bullet::getX() const {
    return _xBullet;
}

int Bullet::getY() const {
    return _yBullet;
}

bool Bullet::isActive() const {
    return _active;
}