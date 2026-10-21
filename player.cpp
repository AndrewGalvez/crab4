#include "player.hpp"
#include "camera.hpp"
#include "components.hpp"
#include "game_object.hpp"
#include "physics.hpp"
#include "raymath.h"
#include "renderers.hpp"
#include "scene.hpp"
#include <raylib.h>

void PlayerController::update(float dt) {
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;
  float speed = stats.speed;
  // movement --------------------------
  if (IsKeyDown(KEY_A))
    t->position.x -= speed * dt;
  if (IsKeyDown(KEY_W))
    t->position.y -= speed * dt;
  if (IsKeyDown(KEY_S))
    t->position.y += speed * dt;
  if (IsKeyDown(KEY_D))
    t->position.x += speed * dt;
  // shooting
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (!owner->scene->getActiveCamera())
      return;

    auto &b = owner->scene->create();
    b.scene = owner->scene;
    float sx = 20;
    float sy = 20;
    float x = (t->position.x + (t->scale.x / 2)) - sx / 2;
    float y = (t->position.y + (t->scale.y / 2)) - sy / 2;

    b.add<c_Transform>(Vector2{x, y}, Vector2{sx, sy});
    Vector2 v = Vector2Scale(
        Vector2Normalize(Vector2Subtract(
            GetScreenToWorld2D(GetMousePosition(),
                               owner->scene->findAllWithComponent<c_Camera>()[0]
                                   ->get<c_Camera>()
                                   ->raw()),
            (Vector2){x, y})),
        stats.bulletSpeed);

    b.add<Velocity>(v);
    b.add<Lifetime>(100);
    b.add<PhysicsManager>();
    b.add<RectangleRenderer>((Rectangle){x, y, sx, sy}, BLUE);
    b.add<Bullet>(stats.bulletDamage);
  }
};
