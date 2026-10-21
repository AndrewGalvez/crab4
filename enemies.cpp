#include "enemies.hpp"
#include "component.hpp"
#include "game_object.hpp"
#include <cmath>

void EnemyController::update(float dt) {
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;

  float dx = target->position.x - t->position.x;
  float dy = target->position.y - t->position.y;
  float m = std::sqrt(dx * dx + dy * dy);
  if (m < 0.0001f)
    return;

  t->position.x += dx / m * (speed * dt);
  t->position.y += dy / m * (speed * dt);
};
