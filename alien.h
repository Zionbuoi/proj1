#ifndef ALIEN_H
#define ALIEN_H

#include "N5110.h"
#include "sprites.h"
#include "mbed.h"
#include <memory> // Included this to better manage how information on bullet location is stored
#include "bullet.h"
#include <vector>
#include "constants.h"

class Alien {
public:
    // so there's going to be 3 rows given the 3 alien types arranged in 5 columns, which I think is fairly optimum given the limited scree  space
    Alien(
        int rows = AlienConfig::ALIEN_ROWS,
        int cols = AlienConfig::ALIEN_COLS,
        int start_x = AlienConfig::ALIEN_START_X,
        int start_y = AlienConfig::ALIEN_START_Y,
        int spacing_x = AlienConfig::ALIEN_SPACING_X,
        int spacing_y = AlienConfig::ALIEN_SPACING_Y
    );
    
    void draw(N5110 &lcd);
    void update();

    // Collision detection
    bool checkCollision(int bullet_x, int bullet_y);
    
    // Getters
    int getRows() const { return _rows; }
    int getCols() const { return _cols; }
    const std::vector<std::unique_ptr<Bullet>>& getBullets() const;
    int get_lowest_y() const;
    
private: //     Alien unit basically handles explosion animations. 
    std::vector<std::unique_ptr<Bullet>> _alienBullets;
    struct AlienUnit {
        bool alive = true;  // Obviously sets "alive" to default
        bool exploding = false;
        Timer explosionTimer;   // controls duration of explosion animation
    };


    void shootBullet();
    AlienUnit _units[3][5];  // Max 3 rows x 5 cols aliens


    // Here are the timers I implemented to control and animate the movement of the aliens array
    Timer _moveTimer;
    Timer _animateTimer;
    Timer _dropTimer;
    Timer _shootTimer;
    bool _moveState;
    bool _dropState;


    int _rows, _cols;
    int _start_x, _start_y;
    int _spacing_x, _spacing_y;
    int _direction;
    int _speed;
};

#endif