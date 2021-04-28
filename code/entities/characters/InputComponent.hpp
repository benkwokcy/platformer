#pragma once

#include "Input.hpp"

class Character;
class Tilemap;
class Level;

class InputComponent {
public:
    virtual ~InputComponent() {};
    virtual void handle_event(Character& me, InputEvent e) = 0;
    virtual void tick(Character& me, Level& level) = 0;
};

class PlayerInputComponent : public InputComponent {
public:
    void handle_event(Character& me, InputEvent e) override;
    void tick(Character& me, Level& level) override;
};

class PigInputComponent : public InputComponent {
public:
    PigInputComponent(int left_boundary, int right_boundary);
    void handle_event(Character& me, InputEvent e) override;
    void tick(Character& me, Level& level) override;
private:
    int left_boundary, right_boundary;
};
