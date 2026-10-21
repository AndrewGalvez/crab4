#include "enemies.hpp"
#include "component.hpp"
#include "components.hpp"
#include "game_object.hpp"
#include "renderers.hpp"
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

void DeadEnemy::update(float dt) {
  auto *l = owner->get<Lifetime>();
  auto *rr = owner->get<RectangleRenderer>();

  if (!l || !rr)
    return;

  rr->opacity =
      (float)((float)l->frames_left / (float)l->frames_start) * 255.0f;
};
