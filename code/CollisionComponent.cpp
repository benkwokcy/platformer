#include "SDL2/SDL.h"

#include "Player.hpp"
#include "Collision.hpp"

#include "CollisionComponent.hpp"

void CollisionComponent::collide_map(Player& player, const SDL_Rect& other) {
    SDL_Rect me = player.bounding_box();
    switch (rect_collide_rect(me, other)) {
        case CollisionType::OVERLAP_TOP:
            player.y += (other.y + other.h) - player.y;
            player.speed_y = std::max(player.speed_y, 0.0f);
            break;
        case CollisionType::OVERLAP_BOTTOM:
            player.y -= (player.y + player.h) - other.y;
            player.on_ground = true;
            player.speed_y = std::min(player.speed_y, 0.0f);
            break;
        case CollisionType::OVERLAP_LEFT:
            player.x += (other.x + other.w) - player.x;
            player.speed_x = 0.0f;
            break;
        case CollisionType::OVERLAP_RIGHT:
            player.x -= (player.x + player.w) - other.x;
            player.speed_x = 0.0f;
            break;
        case CollisionType::TOUCH_BOTTOM:
            player.on_ground = true;
            break;
        default:
            break;
    }
}