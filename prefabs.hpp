#pragma once

#include "game_object.hpp"
#include "physics.hpp"
#include "player.hpp"
#include <raylib.h>

namespace prefabs {
GameObject &enemy(Scene &scene, float x, float y, float w, float h,
                  c_Transform *target);
GameObject &bullet(Scene &scene, Vector2 pos, float damage, float speed);
GameObject &gold(Scene &scene, Vector2 center);
GameObject &deadEnemy(Scene &scene, Rectangle rect);
GameObject &player(Scene &scene, PlayerStats *stats);
}; // namespace prefabs
