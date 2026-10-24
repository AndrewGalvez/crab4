#include "player.hpp"
#include "camera.hpp"
#include "components.hpp"
#include "game_object.hpp"
#include "physics.hpp"
#include "prefabs.hpp"
#include "raymath.h"
#include "renderers.hpp"
#include "scene.hpp"
#include <raylib.h>

void PlayerController::update(float dt) {
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;
  float speed = stats->speed;
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
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (!owner->scene->getActiveCamera())
      return;

    float sx = 20;
    float sy = 20;
    float x = (t->position.x + (t->scale.x / 2)) - sx / 2;
    float y = (t->position.y + (t->scale.y / 2)) - sy / 2;

    prefabs::bullet(*owner->scene, Vector2{x, y}, stats->bulletDamage,
                    stats->bulletSpeed);
  }

  // regen
  if ((!(this->stats->health <= 0)) &&
      (this->stats->health <= this->stats->maxHealth))
    this->stats->health += this->stats->health + 0.025 > this->stats->maxHealth
                               ? this->stats->maxHealth - this->stats->health
                               : 0.025;
};

void PlayerHealthbar::drawUI() {
  if (!health || !maxHealth)
    return;
  float w = 200;
  float h = 20;
  float y = 540;
  float x = 800 / 2.0f - w / 2.0f;
  DrawRectangle(x - 8, y - 8, w + 16, h + 16, ColorAlpha(BLACK, 0.2));
  DrawRectangle(x, y, w, h, GRAY);
  DrawRectangle(x + 2, y + 2, w - 4, h - 4, RED);
  if (*health < 0)
    return;
  DrawRectangle(x + 2, y + 2, -4 + w * (*health / *maxHealth), h - 4, GREEN);
};
