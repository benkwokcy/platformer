#pragma once

#include <string>

#include "Sprite.hpp"

// TODO - does this have to be a singleton?
// We load a bunch of sprites that can be shared between AnimatedSprites.
class Assets {
public:
    static Assets& instance() {
        static Assets* instance_ = new Assets();
        return *instance_;
    }

    // Don't show these file paths if I make my code public on Github
    const static inline std::string PATH = "/Users/benkwok/Desktop/school.nosync/platformer/assets/";
    Sprite player_run;
    Sprite player_idle;
    Sprite player_attack;
    Sprite player_jump;
    Sprite player_fall;
    Sprite player_ground;
    Sprite player_dead;
    Sprite player_hit;
    Sprite pig_run;
    Sprite pig_idle;
    Sprite pig_attack;
    Sprite pig_jump;
    Sprite pig_fall;
    Sprite pig_ground;
    Sprite pig_dead;
    Sprite pig_hit;
    Sprite health_bar;
    Sprite heart_idle;
    Sprite heart_hit;
    Sprite door_idle;
    Sprite door_opening;
private:
    Assets() :
        player_run(PATH + "images/playerrun.png", 624, 58, 78, 58, 16, 28, 23, 16),
        player_idle(PATH + "images/playeridle.png", 858, 58, 78, 58, 16, 28, 23, 16),
        player_attack(PATH + "images/playerattack.png", 234, 58, 78, 58, 16, 28, 23, 16),
        player_jump(PATH + "images/playerjump.png", 78, 58, 78, 58, 16, 28, 23, 16),
        player_fall(PATH + "images/playerfall.png", 78, 58, 78, 58, 16, 28, 23, 16),
        player_ground(PATH + "images/playerground.png", 78, 58, 78, 58, 16, 28, 23, 16),
        player_dead(PATH + "images/playerdead.png", 312, 58, 78, 58, 16, 28, 23, 16, false),
        player_hit(PATH + "images/playerhit.png", 156, 58, 78, 58, 16, 28, 23, 16),
        pig_run(PATH + "images/pigrun.png", 204, 28, 34, 28, 15, 16, 12, 12, true),
        pig_idle(PATH + "images/pigidle.png", 374, 28, 34, 28, 15, 16, 12, 12, true),
        pig_attack(PATH + "images/pigattack.png", 170, 28, 34, 28, 15, 16, 12, 12, true),
        pig_jump(PATH + "images/pigjump.png", 34, 28, 34, 28, 15, 16, 12, 12, true),
        pig_fall(PATH + "images/pigfall.png", 34, 28, 34, 28, 15, 16, 12, 12, true),
        pig_ground(PATH + "images/pigground.png", 34, 28, 34, 28, 15, 16, 12, 12, true),
        pig_dead(PATH + "images/pigdead.png", 136, 28, 34, 28, 15, 16, 12, 12, true),
        pig_hit(PATH + "images/pighit.png", 68, 28, 34, 28, 15, 16, 12, 12, true),
        health_bar(PATH + "images/playerhealthbar.png", 66, 34),
        heart_idle(PATH + "images/smallheartidle.png", 144, 14, 18, 14, 8, 7, 7, 4),
        heart_hit(PATH + "images/smallhearthit.png", 36, 14, 18, 14, 8, 7, 7, 4),
        door_idle(PATH + "images/dooridle.png", 46, 56, 46, 56, 46, 56, 0, 0),
        door_opening(PATH + "images/dooropening.png", 230, 56, 46, 56, 46, 56, 0, 0)
    {
        player_attack.add_collision(0, { 49, 6, 29, 38 });
        player_attack.add_collision(1, { 42, 4, 29, 40 });
        pig_attack.add_collision(2, { 1, 2, 9, 20 });
        pig_attack.add_collision(3, { 5, 0, 8, 10 });
    }
};