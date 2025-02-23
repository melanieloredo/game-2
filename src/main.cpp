#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_sprite_animate_actions.h"
//#include "bn_sprite_items_ninja.h"
#include "bn_sprite_items_lamb.h"
#include "bn_sprite_items_coin_animated.h"
#include "bn_random.h"
#include "bn_sound_items.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_regular_bg_items_room1_bg.h"

void sprites_animation_actions_scene(bn::random randGen) {
    //create background
    bn::regular_bg_ptr bg = bn::regular_bg_items::room1_bg.create_bg(0, 0);
    
    // Create the player sprite
    bn::sprite_ptr lamb_sprite = bn::sprite_items::lamb.create_sprite(0, 0);
    lamb_sprite.set_scale(1); //set correct size for 32

    // Create an animated coin sprite
    bn::sprite_ptr coin_sprite = bn::sprite_items::coin_animated.create_sprite(
        randGen.get_int(-110, 110), randGen.get_int(-80, 80)
    );

    // Create animation action for the coin (looping through frames 0-7)
    bn::sprite_animate_action<8> coin_animation = bn::create_sprite_animate_action_forever(
        coin_sprite, 10, bn::sprite_items::coin_animated.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7
    );

    // Animation variables
    bn::optional<bn::sprite_animate_action<4>> action;
    int last_direction = 3; // Default to facing forward (down)

    while (!bn::keypad::start_pressed()) {
        bool moving = false;
        bool move_left = bn::keypad::left_held();
        bool move_right = bn::keypad::right_held();
        bool move_up = bn::keypad::up_held();
        bool move_down = bn::keypad::down_held();

        randGen.get();

        // If two opposite directions are pressed, stop movement
        if ((move_left && move_right) || (move_up && move_down)) {
            move_left = move_right = move_up = move_down = false;
        }

        // Determine priority direction for diagonal movement
        int new_direction = last_direction;
        if (move_up) new_direction = 2;
        if (move_down) new_direction = 3;
        if (move_left) new_direction = 0;
        if (move_right) new_direction = 1;

        // Move and set animations
        if (move_left) {
            lamb_sprite.set_x(lamb_sprite.x() - 1);
            moving = true;
        }
        if (move_right) {
            lamb_sprite.set_x(lamb_sprite.x() + 1);
            moving = true;
        }
        if (move_up) {
            lamb_sprite.set_y(lamb_sprite.y() - 1);
            moving = true;
        }
        if (move_down) {
            lamb_sprite.set_y(lamb_sprite.y() + 1);
            moving = true;
        }

        // Play animation when moving
        if (moving) {
            if (new_direction != last_direction || !action.has_value()) {
                if (new_direction == 0) { // Left
                    action = bn::create_sprite_animate_action_forever(
                        lamb_sprite, 6, bn::sprite_items::lamb.tiles_item(), 8, 9, 10, 11);
                } 
                else if (new_direction == 1) { // Right
                    action = bn::create_sprite_animate_action_forever(
                        lamb_sprite, 6, bn::sprite_items::lamb.tiles_item(), 12, 13, 14, 15);
                } 
                else if (new_direction == 2) { // Up
                    action = bn::create_sprite_animate_action_forever(
                        lamb_sprite, 6, bn::sprite_items::lamb.tiles_item(), 4, 5, 6, 7);
                } 
                else if (new_direction == 3) { // Down
                    action = bn::create_sprite_animate_action_forever(
                        lamb_sprite, 6, bn::sprite_items::lamb.tiles_item(), 0, 1, 2, 3);
                }
                last_direction = new_direction;
            }
            action->update(); // Update animation
        }

        // If no movement, set standing frame based on last movement direction
        if (!moving) {
            if (action.has_value()) {
                action.reset(); // Stop animation
                int idle_frame = (last_direction == 0) ? 8  // Left
                               : (last_direction == 1) ? 12 // Right
                               : (last_direction == 2) ? 4  // Up
                               : 0; // Down (default)
                lamb_sprite.set_tiles(bn::sprite_items::lamb.tiles_item(), idle_frame);
            }
        }

        // **Update animated coin**
        coin_animation.update();

        // **Check for Collision & Respawn Coin**
        int dx = lamb_sprite.x().integer() - coin_sprite.x().integer();
        int dy = lamb_sprite.y().integer() - coin_sprite.y().integer();
        int distance_squared = (dx * dx) + (dy * dy);

        if (distance_squared < 32) { // Collision detected
            bn::sound_items::coin.play(); // Play the coin sound
            
            // Respawn the coin
            coin_sprite.set_x(randGen.get_int(-110, 110));
            coin_sprite.set_y(randGen.get_int(-80, 80));
        }

        bn::core::update();
    }
}

int main() {
    bn::random random_generator;
    bn::core::init();
    while (true) {
        random_generator.get();
        sprites_animation_actions_scene(random_generator);
        bn::core::update();
    }
}
