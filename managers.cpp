#include "managers.hpp"
#include "camera.hpp"
#include "components.hpp"
#include "enemies.hpp"
#include "game.hpp"
#include "gold.hpp"
#include "player.hpp"
#include "prefabs.hpp"
#include "raylib.h"
#include "scene.hpp"
#include "tilemap.hpp"
#include "utils.hpp"
#include <raymath.h>

void CollisionManager::update(float dt) {
  auto bullets = owner->scene->findAllWithComponent<Bullet>();
  auto targets = owner->scene->findAllWithComponent<Health>();
  auto *map = owner->scene->findFirstWithComponent<TileMap>();
  auto walls = owner->scene->findAllWithComponent<Wall>();
  auto solids = owner->scene->findAllWithComponent<Solid>();
  auto enemies = owner->scene->findAllWithComponent<EnemyController>();
  auto golds = owner->scene->findAllWithComponent<Gold>();
  auto *player = owner->scene->findFirstWithComponent<PlayerController>();
  auto doors = owner->scene->findAllWithComponent<Door>();
  if (!player)
    return;

  for (auto *d : doors) {
    if (d->get<c_Transform>()) {
      if (CheckCollisionRecs(rectOf(player), rectOf(d))) {
        auto dr = d->get<Door>();
        owner->scene->game->switchTo(dr->dest);
        owner->scene->game->state.spawnPos = dr->arrivesAt;
      }
    }
  }

  for (auto *b : bullets) {
    if (b->destroyed || !b->get<c_Transform>())
      continue;
    Rectangle br = rectOf(b);

    if (map)
      if (auto *mr = map->get<TileMap>())
        if (mr->isSolidAtPoint(rectCenter(br))) {
          owner->scene->destroy(b);
          continue;
        }

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

          auto *et = e->get<c_Transform>();
          prefabs::gold(*owner->scene,
                        Vector2{et->position.x + et->scale.x / 2,
                                et->position.y + et->scale.y / 2});

          prefabs::deadEnemy(*owner->scene, rectOf(e));
        }
        break;
      }
    }
  }

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

  if (map) {
    auto *mapraw = map->get<TileMap>();
    for (auto *s : owner->scene->findAllWithComponent<Solid>())
      mapraw->collide(s);
  }
};

void GameManager::update(float dt) {
  frame++;
  if (frame > 40) {
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

    prefabs::enemy(*owner->scene, x, y, w, h, p->get<c_Transform>());
  }
};
