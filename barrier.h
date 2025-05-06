#ifndef BARRIER_H
#define BARRIER_H

#include "N5110.h"
#include "sprites.h"
#include <vector>

class Barrier {
public:
    Barrier(int count, int start_x, int start_y, int spacing);

    void draw(N5110 &lcd);
    bool check_collision(int x, int y);
    void take_damage(int x, int y);
    bool aliens_reached(int alien_y) const;

private:
    struct BarrierUnit {
        int x, y;
        int health = 100;
    };
    
    std::vector<BarrierUnit> _units;
};

#endif