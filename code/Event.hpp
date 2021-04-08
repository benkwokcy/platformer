#pragma once

// The different event types in this game.
enum class Event {
    NONE,  // no events in the SDL queue
    CONTINUE,
    QUIT,
    LEFT_PRESS,
    LEFT_RELEASE,
    RIGHT_PRESS,
    RIGHT_RELEASE,
    ATTACK,
    IGNORE  // an SDL event that we don't care about
};

// Get the next SDL event and convert it into an in-game event.
Event get_event();