#include "gold.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "physics.hpp"
#include "player.hpp"
#include "scene.hpp"
#include <cstdio>
#include <raymath.h>

void Gold::update(float dt) {
  auto *v = owner->get<Velocity>();
  auto *p = owner->scene->findFirstWithComponent<PlayerController>();
  auto *pt = p->get<c_Transform>();
  auto *t = owner->get<c_Transform>();
  float m = owner->scene->game->state.pstats.magnetRadius;

  if (!v || !p || !t) {
    return;
  }

  // distance
  float d = Vector2Distance(
      {t->position.x + t->scale.x / 2, t->position.y + t->scale.y / 2},
      {pt->position.x + pt->scale.x / 2, pt->position.y + pt->scale.y / 2});

  if (m >= d) {
    float dx =
        pt->position.x + pt->scale.x / 2 - (t->position.x + t->scale.x / 2);
    float dy =
        pt->position.y + pt->scale.y / 2 - (t->position.y + t->scale.y / 2);
    float m = std::sqrt(dx * dx + dy * dy);
    if (m < 0.0001f)
      return;

    t->position.x += dx / m * (speed * dt);
    t->position.y += dy / m * (speed * dt);
  }
};
