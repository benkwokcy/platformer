#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdexcept>

#include "Entity.hpp"
#include "Window.hpp"
#include "Sprite.hpp"

// The main menu.
class Menu : public Entity {
public:
    Menu() :
        sprite(Sprite("sprites/mainmenu.png"))
    {}

    void paint() const override {
        sprite.paint();
    }

    void handle_event(Event e) override {}
    void tick() override {}

private:
    Sprite sprite;
};