#include "managers.hpp"
#include "camera.hpp"
#include "components.hpp"
#include "enemies.hpp"
#include "player.hpp"
#include "raylib.h"
#include "renderers.hpp"
#include "scene.hpp"
#include "utils.hpp"

void CollisionManager::update(float dt) {
  auto bullets = owner->scene->findAllWithComponent<Bullet>();
  auto targets = owner->scene->findAllWithComponent<Health>();

  for (auto *b : bullets) {
    if (b->destroyed || !b->get<c_Transform>())
      continue;
    Rectangle br = rectOf(b);

    for (auto *e : targets) {
      if (e->destroyed || !e->get<c_Transform>())
        continue;

      if (CheckCollisionRecs(br, rectOf(e))) {
        auto *hp = e->get<Health>();
        hp->value -= b->get<Bullet>()->damage;
        e->get<Velocity>()->value = b->get<Velocity>()->value;
        owner->scene->destroy(b);
        if (hp->value <= 0)
          owner->scene->destroy(e);
        break;
      }
    }
  };
}

void GameManager::update(float dt) {
  frame++;
  // spawn three enemies every second
  if (frame > 20) {
    frame = 0;

    auto *p = owner->scene->findFirstWithComponent<PlayerController>();
    if (!p)
      return;

    auto *c = owner->scene->findFirstWithComponent<c_Camera>();
    if (!c)
      return;

    auto cam = c->get<c_Camera>()->camera;

    float camera_left_x = cam.target.x - cam.offset.x / cam.zoom;
    float camera_right_x =
        cam.target.x + (GetScreenWidth() - cam.offset.x) / cam.zoom;
    float camera_top_y = cam.target.y - cam.offset.y / cam.zoom;
    float camera_bottom_y =
        cam.target.y + (GetScreenHeight() - cam.offset.y) / cam.zoom;

    // spawning position
    int top_left_right_bottom_cam = GetRandomValue(0, 3);

    float x, y, w, h;
    w = 100;
    h = 100;

    switch (top_left_right_bottom_cam) {
    case 0:
      // top
      x = GetRandomValue(camera_left_x - w, camera_right_x);
      y = camera_top_y - h - GetRandomValue(10, 40);
      break;
    case 1:
      // left
      x = camera_left_x - w - GetRandomValue(10, 40);
      y = GetRandomValue(camera_top_y - h, camera_bottom_y);
      break;
    case 2:
      // right
      x = camera_right_x + GetRandomValue(10, 40);
      y = GetRandomValue(camera_top_y - h, camera_bottom_y);
      break;
    case 3:
      // bottom
      x = GetRandomValue(camera_left_x - w, camera_right_x);
      y = camera_bottom_y + h + GetRandomValue(10, 40);
      break;
    }

    auto &e = owner->scene->create();
    e.add<c_Transform>(Vector2{x, y}, Vector2{w, h});
    e.add<RectangleRenderer>(Rectangle{x, y, w, h}, GREEN);
    e.add<EnemyController>(200.0f, p->get<c_Transform>());
    e.add<Health>(50);
    e.add<Velocity>(Vector2{0, 0});
    e.add<PhysicsManager>();
    e.add<Friction>(200.0f);
  }
};
