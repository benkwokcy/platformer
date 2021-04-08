#pragma once

#include "Sprite.hpp"
#include "Event.hpp"

class Player;

class PlayerState {
public:
    virtual void enter(Player& player) = 0;
    virtual bool handle_event(Player& player, Event e) = 0;
    virtual void paint(Player& player) = 0;
    virtual void tick(Player& player) = 0;
};

class PlayerRunLeft : public PlayerState {
public:
    Sprite sprite;
    PlayerRunLeft();
    void enter(Player& player) override;
    bool handle_event(Player& player, Event e) override;
    void paint(Player& player) override;
    void tick(Player& player) override;
};

class PlayerRunRight : public PlayerState {
public:
    Sprite sprite;
    PlayerRunRight();
    void enter(Player& player) override;
    bool handle_event(Player& player, Event e) override;
    void paint(Player& player) override;
    void tick(Player& player) override;
};

class PlayerIdle : public PlayerState {
public:
    Sprite sprite;
    PlayerIdle();
    void enter(Player& player) override;
    bool handle_event(Player& player, Event e) override;
    void paint(Player& player) override;
    void tick(Player& player) override;
};

class PlayerAttack : public PlayerState {
public:
    Sprite sprite;
    PlayerAttack();
    void enter(Player& player) override;
    bool handle_event(Player& player, Event e) override;
    void paint(Player& player) override;
    void tick(Player& player) override;
};