#include "alien.h"
#include "N5110.h"
#include "sprites.h"
#include "mbed.h"
#include "bullet.h"
#include <memory>
#include <vector>
#include "constants.h"
#include <algorithm>

Alien::Alien(int rows, int cols, int start_x, int start_y, 
            int spacing_x, int spacing_y)
    : _rows(rows), _cols(cols), _start_x(start_x), _start_y(start_y),
      _spacing_x(spacing_x), _spacing_y(spacing_y), _direction(1), 
      _speed(1), _moveState(0), _dropState(0) {
    _moveTimer.start();
    _animateTimer.start();
    _dropTimer.start();
    _shootTimer.start();
}

void Alien::update() {
    if (_moveTimer.elapsed_time() >= 1000ms) {
        _start_x += _direction * _speed;
        _moveTimer.reset();
        _moveState = !_moveState;

        // Calculate rightmost position
        const int sprite_width = (_rows == 0) ? 10 : 
                                (_rows == 1) ? 12 : 8;
        const int rightmost_x = _start_x + 
                               ((_cols - 1) * _spacing_x) + 
                               sprite_width;

        // Screen edge check
        if (rightmost_x >= 84 || _start_x <= 0) {
            _direction *= -1;
            _dropState = true;
            _dropTimer.reset();
            _start_y += 2;  // Move down when hitting edge
        }
    }

    if (_dropState && _dropTimer.elapsed_time() > 500ms) {
        _dropState = false;
    }

    if (_shootTimer.elapsed_time() >= 2000ms) {
        shootBullet();
        _shootTimer.reset();
    }

    for (auto& bullet : _alienBullets) {
        bullet->update();
    }

    _alienBullets.erase(
        std::remove_if(_alienBullets.begin(), _alienBullets.end(),
            [](const std::unique_ptr<Bullet>& b) { 
                return !b->isActive(); 
            }),
        _alienBullets.end());
}

void Alien::shootBullet() {
    std::vector<std::pair<int, int>> aliveAliens;
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _cols; col++) {
            if (_units[row][col].alive && !_units[row][col].exploding) {
                int x = _start_x + col * _spacing_x;
                int y = _start_y + row * _spacing_y;
                aliveAliens.emplace_back(x, y);
            }
        }
    }

 if (!aliveAliens.empty()) {
        int index = rand() % aliveAliens.size();
        auto bullet = std::make_unique<Bullet>();
        bullet->shoot(
            aliveAliens[index].first, 
            aliveAliens[index].second, 
            1,         // Direction = down
            false      // Skip cooldown check
        );
        _alienBullets.push_back(std::move(bullet));
    }
}

int Alien::get_lowest_y() const {
    // Returns bottom Y of lowest alien row
    return _start_y + ((_rows - 1) * _spacing_y) + 8;  // 8 = sprite height
}

const std::vector<std::unique_ptr<Bullet>>& Alien::getBullets() const {
    return _alienBullets;
}

void Alien::draw(N5110 &lcd) { 
    for (int row = 0; row < _rows; row++) {
    for (int col = 0; col < _cols; col++) {
        AlienUnit &unit = _units[row][col];
        if (!unit.alive) continue;

        int x = _start_x + col * _spacing_x;
        int y = _start_y + row * _spacing_y;

        int* sprite_ptr = nullptr;
        int width = 0;
        int height = 8;

        if (unit.exploding) {
            if (unit.explosionTimer.elapsed_time() > 400ms) {
                unit.exploding = false;
                unit.alive = false;
                continue; // Skip drawing this alien now
            } else {
                sprite_ptr = reinterpret_cast<int*>(const_cast<int(*)[13]>(explode));
                width = 13;
                height = 7;
            }
        }
        else if (_dropState && _dropTimer.elapsed_time() <= 500ms) {
            switch (row) {
                case 0:
                    sprite_ptr = reinterpret_cast<int*>(const_cast<int(*)[10]>(alien_topDrop));
                    width = 10;
                    break;
                case 1:
                    sprite_ptr = reinterpret_cast<int*>(const_cast<int(*)[12]>(alien_midDrop));
                    width = 12;
                    break;
                default:
                    sprite_ptr = reinterpret_cast<int*>(const_cast<int(*)[8]>(alien_bottomDrop));
                    width = 8;
            }
        }
        else {
            switch (row) {
                case 0:
                    sprite_ptr = _moveState 
                        ? reinterpret_cast<int*>(const_cast<int(*)[10]>(alien_topMove))
                        : reinterpret_cast<int*>(const_cast<int(*)[10]>(alien_top));
                    width = 10;
                    break;
                case 1:
                    sprite_ptr = _moveState 
                        ? reinterpret_cast<int*>(const_cast<int(*)[12]>(alien_midMove))
                        : reinterpret_cast<int*>(const_cast<int(*)[12]>(alien_mid));
                    width = 12;
                    break;
                default:
                    sprite_ptr = _moveState 
                        ? reinterpret_cast<int*>(const_cast<int(*)[8]>(alien_bottomMove))
                        : reinterpret_cast<int*>(const_cast<int(*)[8]>(alien_bottom));
                    width = 8;
            }
        }

        lcd.drawSprite(x, y, height, width, sprite_ptr);
    }
}
}

bool Alien::checkCollision(int bullet_x, int bullet_y) {
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _cols; col++) {
            AlienUnit &unit = _units[row][col];
            if (!unit.alive || unit.exploding) continue;

            int x = _start_x + col * _spacing_x;
            int y = _start_y + row * _spacing_y;
            int w = (row == 0) ? 10 : (row == 1) ? 12 : 8;
            int h = 8;

            if (bullet_x >= x && bullet_x < x + w &&
                bullet_y >= y && bullet_y < y + h) {
                unit.exploding = true;
                unit.explosionTimer.reset();
                unit.explosionTimer.start();
                return true;
            }
        }
    }
    return false;
}