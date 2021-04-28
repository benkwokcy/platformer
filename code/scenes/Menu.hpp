#pragma once

#include <SDL2/SDL.h>

#include <stdexcept>

#include "Scene.hpp"
#include "Sprite.hpp"
#include "Assets.hpp"

class Menu : public Scene {
public:
    Menu() :
        sprite(Sprite(Assets::PATH + "images/startmenu.png", 640, 480))
    {}

    void paint() override {
        sprite.paint();
    }

    void handle_event(InputEvent e) override {}
    void tick() override {}
    void enter() override {}

private:
    Sprite sprite;
};