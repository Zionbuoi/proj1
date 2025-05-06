#include "barrier.h"
#include "N5110.h"

Barrier::Barrier(int count, int start_x, int start_y, int spacing) {
    for(int i = 0; i < count; i++) {
        _units.push_back({
            start_x + (i * spacing),  // X position
            start_y,                  // Y position
            100                       // Initial health
        });
    }
}

void Barrier::draw(N5110 &lcd) {
    for(size_t i = 0; i < _units.size(); i++) {
        if(_units[i].health > 0) {
            int visible_rows = 4 * (_units[i].health / 100.0f);
            lcd.drawSprite(_units[i].x, _units[i].y, 
                          visible_rows, 9, (int*)barrier);
        }
    }
}

bool Barrier::check_collision(int x, int y) {
    for(size_t i = 0; i < _units.size(); i++) {
        if(_units[i].health <= 0) continue;
        
        if(x >= _units[i].x && 
           x < _units[i].x + 9 &&
           y >= _units[i].y && 
           y < _units[i].y + 4) {
            return true;
        }
    }
    return false;
}

void Barrier::take_damage(int x, int y) {
    for(size_t i = 0; i < _units.size(); i++) {
        if(_units[i].health <= 0) continue;
        
        if(x >= _units[i].x && 
           x < _units[i].x + 9 && 
           y >= _units[i].y && 
           y < _units[i].y + 4) {
            _units[i].health = (_units[i].health > 5) ? _units[i].health - 5 : 0;
            break;
        }
    }
}

bool Barrier::aliens_reached(int alien_y) const {
    return alien_y >= _units[0].y;
}