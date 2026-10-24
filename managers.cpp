#include "managers.hpp"
#include "camera.hpp"
#include "components.hpp"
#include "enemies.hpp"
#include "game.hpp"
#include "gold.hpp"
#include "player.hpp"
#include "raylib.h"
#include "renderers.hpp"
#include "scene.hpp"
#include "utils.hpp"
#include <raymath.h>

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
        // play sound
        owner->scene->asset_manager.playSound(
            (const char *[3]){"assets/hit1.wav", "assets/hit2.wav",
                              "assets/hit3.wav"}[GetRandomValue(0, 2)]);

        auto *hp = e->get<Health>();
        hp->value -= b->get<Bullet>()->damage;
        e->get<Velocity>()->value =
            Vector2Add(e->get<Velocity>()->value, b->get<Velocity>()->value);
        owner->scene->destroy(b);
        if (hp->value <= 0) {
          owner->scene->destroy(e);
          auto &d = owner->scene->create();
          d.add<RectangleRenderer>(e->get<RectangleRenderer>()->rect,
                                   Color{255, 0, 0, 255});
          d.add<Lifetime>(60);
          d.add<DeadEnemy>();

          auto *et = e->get<c_Transform>();

          owner->scene->create()
              ._add<Gold>()
              ._add<c_Transform>(Vector2{et->position.x + et->scale.x / 2 - 5,
                                         et->position.y + et->scale.y / 2 - 5},
                                 Vector2{40, 40})
              ._add<Lifetime>(60000)
              ._add<Velocity>(Vector2{0, 0})
              ._add<ImageRenderer>(
                  "assets/gold.png",
                  Rectangle{et->position.x + et->scale.x / 2 - 5,
                            et->position.y + et->scale.y / 2 - 5, 40, 40},
                  Rectangle{0, 0, 16, 16}, 0, 10, Vector2{0, 0}, 0.0f,
                  [](ImageRenderer &self) {
                    self.source = Rectangle{16.0f * self.animFrame, 0, 16, 16};
                  },
                  0, 8);
        }
        break;
      }
    }
  }

  auto enemies = owner->scene->findAllWithComponent<EnemyController>();
  auto golds = owner->scene->findAllWithComponent<Gold>();
  auto *player = owner->scene->findFirstWithComponent<PlayerController>();
  for (auto *e : enemies) {
    if (e->destroyed || !e->get<c_Transform>())
      continue;
    if (CheckCollisionRecs(rectOf(player), rectOf(e))) {
      owner->scene->asset_manager.playSound("assets/hurt.wav");

      c_Transform *pt = player->get<c_Transform>();
      c_Transform *et = e->get<c_Transform>();
      Velocity *ev = e->get<Velocity>();
      player->get<PlayerController>()->stats->health -= 25;
      e->get<Velocity>()->value =
          Vector2Add(ev->value, Vector2Scale(Vector2Normalize(Vector2Subtract(
                                                 et->position, pt->position)),
                                             4000.0f));
      player->get<Velocity>()->value = Vector2Add(
          player->get<Velocity>()->value,
          Vector2Scale(
              Vector2Normalize(Vector2Subtract(et->position, pt->position)),
              -4000.0f));

      if (player->get<PlayerController>()->stats->health <= 0) {
        player->scene->game->switchTo("deadmenu");
      }
    }
  }

  auto *pt = player->get<c_Transform>();
  if (!pt)
    return;

  for (auto *g : golds) {
    if (g->destroyed)
      continue;
    auto *gt = g->get<c_Transform>();
    if (!gt)
      continue;
    if (CheckCollisionRecs(
            Rectangle{gt->position.x, gt->position.y, gt->scale.x, gt->scale.y},
            Rectangle{pt->position.x, pt->position.y, pt->scale.x,
                      pt->scale.y})) {
      g->scene->game->state.pstats.gold++;
      g->scene->destroy(g);
      g->scene->asset_manager.playSound("assets/goldPickup.wav");
    }
  }
};

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
    float camera_right_x = cam.target.x + (800 - cam.offset.x) / cam.zoom;
    float camera_top_y = cam.target.y - cam.offset.y / cam.zoom;
    float camera_bottom_y = cam.target.y + (800 - cam.offset.y) / cam.zoom;

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
