#include "bn_core.h"

#include "bn_keypad.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_ninja.h"

void sprites_animation_actions_scene() {
    bn::sprite_ptr ninja_sprite = bn::sprite_items::ninja.create_sprite(0, 0);
    bn::sprite_animate_action<4> action = bn::create_sprite_animate_action_forever(ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);

    while(! bn::keypad::start_pressed())
    {
        if(bn::keypad::left_pressed())
        {
            action = bn::create_sprite_animate_action_forever(
                        ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 8, 9, 10, 11);
        }
        else if(bn::keypad::right_pressed())
        {
            action = bn::create_sprite_animate_action_forever(
                        ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 12, 13, 14, 15);
        }

        if(bn::keypad::up_pressed())
        {
            action = bn::create_sprite_animate_action_forever(
                        ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 4, 5, 6, 7);
        }
        else if(bn::keypad::down_pressed())
        {
            action = bn::create_sprite_animate_action_forever(
                        ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);
        }

        action.update();
        bn::core::update();
    }
}

int main()
{
    bn::core::init();

    while(true)
    {
        sprites_animation_actions_scene();
        bn::core::update();
    }
}
