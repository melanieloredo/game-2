#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_items_ninja.h"

void sprites_animation_actions_scene() {
    bn::sprite_ptr ninja_sprite = bn::sprite_items::ninja.create_sprite(0, 0);

    // Default standing frame (single frame, not animated)
    ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item(), 0);

    bn::optional<bn::sprite_animate_action<4>> action; // Store active animation
    int last_direction = 3; // Default to facing forward (down)

    while (!bn::keypad::start_pressed()) {
        bool moving = false;
        bool move_left = bn::keypad::left_held();
        bool move_right = bn::keypad::right_held();
        bool move_up = bn::keypad::up_held();
        bool move_down = bn::keypad::down_held();

        // If two opposite directions are pressed, stop movement**
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
            ninja_sprite.set_x(ninja_sprite.x() - 1);
            moving = true;
        }
        if (move_right) {
            ninja_sprite.set_x(ninja_sprite.x() + 1);
            moving = true;
        }
        if (move_up) {
            ninja_sprite.set_y(ninja_sprite.y() - 1);
            moving = true;
        }
        if (move_down) {
            ninja_sprite.set_y(ninja_sprite.y() + 1);
            moving = true;
        }

        // Play animation based on movement direction
        if (moving) {
            if (new_direction != last_direction) {
                if (new_direction == 0) { // Left
                    action = bn::create_sprite_animate_action_forever(
                        ninja_sprite, 6, bn::sprite_items::ninja.tiles_item(), 8, 9, 10, 11);
                } 
                else if (new_direction == 1) { // Right
                    action = bn::create_sprite_animate_action_forever(
                        ninja_sprite, 6, bn::sprite_items::ninja.tiles_item(), 12, 13, 14, 15);
                } 
                else if (new_direction == 2) { // Up
                    action = bn::create_sprite_animate_action_forever(
                        ninja_sprite, 6, bn::sprite_items::ninja.tiles_item(), 4, 5, 6, 7);
                } 
                else if (new_direction == 3) { // Down
                    action = bn::create_sprite_animate_action_forever(
                        ninja_sprite, 6, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);
                }
                last_direction = new_direction;
            }
            if (action.has_value()) {
                action->update(); // Only update if valid
            }
        }

        // If no movement, set standing frame based on last movement direction
        if (!moving) {
            if (action.has_value()) {
                action.reset(); // Stop animation
                int idle_frame = (last_direction == 0) ? 8  // Left
                               : (last_direction == 1) ? 12 // Right
                               : (last_direction == 2) ? 4  // Up
                               : 0; // Down (default)
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item(), idle_frame);
            }
        }

        bn::core::update();
    }
}

int main() {
    bn::core::init();

    while (true) {
        sprites_animation_actions_scene();
        bn::core::update();
    }
}
