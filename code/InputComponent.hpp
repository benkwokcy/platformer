#pragma once

class Player;

class InputComponent {
public:
    void handle_event(Player& player, InputEvent e);
};
