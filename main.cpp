#include "mbed.h"
#include "N5110.h"
#include "mainplayer.h"
#include "alien.h"
#include "Joystick.h"
#include "centre_printstr.h"
#include "bullet.h"
#include "constants.h"
#include "barrier.h"
#include "sprites.h"

N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);
Joystick joystick(PC_3, PC_2);
DigitalIn fire_button(PC_12);
DigitalOut fire_led(PA_15);

int main() {
    // Initialize hardware
    lcd.init(LPH7366_1);
    lcd.setContrast(0.55);
    lcd.setBrightness(0.5);
    joystick.init();  // Critical for calibration

    // Game objects
    Player player(
        (Constants::SCREEN_WIDTH - Constants::PLAYER_WIDTH) / 2,  // Center X
        Constants::SCREEN_HEIGHT - Constants::PLAYER_HEIGHT - Constants::BOTTOM_MARGIN, // Bottom Y
        Constants::PLAYER_WIDTH,
        Constants::PLAYER_HEIGHT,
        (int*)player_ship
    );
    Barrier barriers(3, 14, 35, 23);
    Alien aliens(3, 5, 0, 0, 12, 10);
    Bullet player_bullet;

    bool game_over = false;
    bool exploding = false;
    Timer explosion_timer;

    while(true) {
        lcd.clear();

        if(!game_over && !exploding) {
            // Player movement
            player.move(lcd, joystick);
            
            // Shooting logic
            if(fire_button == 0) {  // Active-low button
                fire_led = 1;
                player_bullet.shoot(
                    player.getX() + 2, 
                    player.getY() - 1, 
                    -1  // Upward direction
                );
            } else {
                fire_led = 0;
            }

            // Update game state
            aliens.update();
            player_bullet.update();

            // Player bullet vs aliens
            if(player_bullet.isActive() && 
               aliens.checkCollision(player_bullet.getX(), player_bullet.getY())) {
                player_bullet.deactivate();
            }

            // Alien bullets vs player/barriers
            const std::vector<std::unique_ptr<Bullet>>& alien_bullets = aliens.getBullets();
            for(size_t i = 0; i < alien_bullets.size(); i++) {
                Bullet* ab = alien_bullets[i].get();
                
                if(ab->isActive()) {
                    // Player collision
                    if(ab->getX() >= player.getX() && 
                       ab->getX() < player.getX() + 5 && 
                       ab->getY() >= player.getY() && 
                       ab->getY() < player.getY() + 5) {
                        exploding = true;
                        explosion_timer.start();
                    }
                    
                    // Barrier collision
                    if(barriers.check_collision(ab->getX(), ab->getY())) {
                        barriers.take_damage(ab->getX(), ab->getY());
                        ab->deactivate();
                        lcd.drawSprite(ab->getX()-3, ab->getY()-3, 7, 13, (int*)explode);
                    }
                }
            }

            // Check alien position
            if(aliens.get_lowest_y() >= 35) {  // 35 = barrier Y position
                game_over = true;
            }
        }

        // Rendering
        aliens.draw(lcd);
        barriers.draw(lcd);
        player_bullet.draw(lcd);
        
        // Draw alien bullets
        const std::vector<std::unique_ptr<Bullet>>& alien_bullets = aliens.getBullets();
        for(size_t i = 0; i < alien_bullets.size(); i++) {
            alien_bullets[i]->draw(lcd);
        }

        if(exploding) {
            if(explosion_timer.elapsed_time() < 1000ms) {
                lcd.drawSprite(player.getX(), player.getY(), 7, 13, (int*)explode);
            } else {
                if(!game_over) {
                    lcd.clear();
                    centerPrintString(lcd, "GAME OVER", 2);
                    lcd.refresh();
                    game_over = true;
                }
            }
        } else {
            lcd.drawSprite(player.getX(), player.getY(), 5, 5, (int*)player_ship);
        }

        lcd.refresh();
        ThisThread::sleep_for(50ms);  // ~20 FPS
    }
}