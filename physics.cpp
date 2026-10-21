#include "physics.hpp"
#include "game_object.hpp"
#include "scene.hpp"

void PhysicsManager::update(float dt) {
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;
  auto *v = owner->get<Velocity>();
  if (!v)
    return;
  t->position.x += v->value.x * dt;
  t->position.y += v->value.y * dt;
  auto *f = owner->get<Friction>();
  if (!f)
    return;
  if (v->value.x > 0) {
    if (v->value.x > f->value) {
      v->value.x -= f->value;
    } else {
      v->value.x = 0;
    }
  } else if (v->value.x < 0) {
    if (v->value.x < -f->value) {
      v->value.x += f->value;
    } else {
      v->value.x = 0;
    }
  }
  if (v->value.y > 0) {
    if (v->value.y > f->value) {
      v->value.y -= f->value;
    } else {
      v->value.y = 0;
    }
  } else if (v->value.y < 0) {
    if (v->value.y < -f->value) {
      v->value.y += f->value;
    } else {
      v->value.y = 0;
    }
  }
}
