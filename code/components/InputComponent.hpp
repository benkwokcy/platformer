#pragma once

class Entity;

class InputComponent {
public:
    void handle_event(Entity& entity, InputEvent e);
};
