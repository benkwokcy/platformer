#include "Window.hpp"
#include "Assets.hpp"
#include "Entity.hpp"
#include "Level.hpp"

#include "HealthBar.hpp"

HealthBar::HealthBar() :
    bar(Assets::instance().health_bar),
    heartidle(AnimatedSprite(Assets::instance().heart_idle)),
    hearthit1(AnimatedSprite(Assets::instance().heart_hit)),
    hearthit2(AnimatedSprite(Assets::instance().heart_hit)),
    hearthit3(AnimatedSprite(Assets::instance().heart_hit)),
    x(Window::width - 66 - 15), y(15),
    isOneHit(false), isTwoHit(false), isThreeHit(false)
{}

void HealthBar::handle_event(LevelEvent event, Level& level, Entity& other) {
    switch(event) {
        case LevelEvent::LOST_HEALTH:
            switch (other.health) {
                case 2:
                    isThreeHit = true;
                    hearthit3.reset_time();
                    break;
                case 1:
                    isTwoHit = true;
                    hearthit2.reset_time();
                    break;
                case 0:
                    isOneHit = true;
                    hearthit1.reset_time();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void HealthBar::paint(int health) {
    bar.paint(x, y);
    if (health > 0) { heartidle.paint(x + 18, y + 14); }
    if (health > 1) { heartidle.paint(x + 29, y + 14); }
    if (health > 2) { heartidle.paint(x + 40, y + 14); }
    if (isOneHit) {
        if (hearthit1.loops_completed()) {
            isOneHit = false;
        } else {
            hearthit1.paint(x + 18, y + 14);
        }
    }
    if (isTwoHit) {
        if (hearthit2.loops_completed()) {
            isTwoHit = false;
        } else {
            hearthit2.paint(x + 29, y + 14);
        }
    }
    if (isThreeHit) {
        if (hearthit3.loops_completed()) {
            isThreeHit = false;
        } else {
            hearthit3.paint(x + 40, y + 14);
        }
    }
}
