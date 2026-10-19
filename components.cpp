#include "components.hpp"
#include "camera.hpp"
#include "component.hpp"
#include "game_object.hpp"
#include "scene.hpp"
#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include <string>

void RectangleRenderer::draw() { DrawRectangleRec(rect, color); }

void RectangleRenderer::update(float dt) {
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;
  rect.x = t->position.x;
  rect.y = t->position.y;
  rect.width = t->scale.x;
  rect.height = t->scale.y;
}

void UIPositionLabel::drawUI() {
  DrawText(value.c_str(), (int)pos.x, (int)pos.y, fontSize, color);
};

void UIPositionLabel::update(float dt) {
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;
  float x = t->position.x;
  float y = t->position.y;
  this->value = "x: " + std::to_string(x) + " y: " + std::to_string(y);
};

UIPositionLabel::UIPositionLabel()
    : UITextRenderer("", RAYWHITE, 20, Vector2{10, 10}) {}

void PlayerController::update(float dt) {
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;
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
    if (owner->scene->findAllWithComponent<c_Camera>().empty())
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
        5000.0f);

    b.add<Velocity>(v);
    b.add<Lifetime>(100);
    b.add<PhysicsManager>();
    b.add<RectangleRenderer>((Rectangle){x, y, sx, sy}, BLUE);
    b.add<Bullet>(1);
  }
};

void BackgroundRenderer::draw() {
  auto cams = owner->scene->findAllWithComponent<c_Camera>();
  if (cams.empty())
    return;
  auto *camComp = cams[0]->get<c_Camera>();
  const Camera2D &cam = camComp->raw();

  // World-space corners of what's currently on screen
  Vector2 tl = GetScreenToWorld2D({0, 0}, cam);
  Vector2 br = GetScreenToWorld2D(
      {(float)GetScreenWidth(), (float)GetScreenHeight()}, cam);

  // Range of tile indices covering that area (floorf handles negatives)
  int x0 = (int)floorf(tl.x / tilesize.x);
  int y0 = (int)floorf(tl.y / tilesize.y);
  int x1 = (int)floorf(br.x / tilesize.x);
  int y1 = (int)floorf(br.y / tilesize.y);

  for (int cy = y0; cy <= y1; cy++) {
    for (int cx = x0; cx <= x1; cx++) {
      Color c = ((cx + cy) & 1) ? color_a : color_b;
      DrawRectangle((int)(cx * tilesize.x), (int)(cy * tilesize.y),
                    (int)tilesize.x, (int)tilesize.y, c);
    }
  }
}

void Lifetime::update(float dt) {
  frames_left--;
  if (frames_left <= 0)
    this->owner->scene->destroy(this->owner);
};

void PhysicsManager::update(float dt) {
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;
  auto *v = owner->get<Velocity>();
  if (!v)
    return;
  t->position.x += v->value.x * dt;
  t->position.y += v->value.y * dt;
}

void EnemyController::update(float dt) {
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;

  float dx = target->position.x - t->position.x;
  float dy = target->position.y - t->position.y;
  float m = std::sqrt(dx * dx + dy * dy);

  t->position.x += dx / m * (speed * dt);
  t->position.y += dy / m * (speed * dt);
};

void GameManager::update(float dt) {
  frame++;
  // spawn an enemy every second
  if (frame > 60) {
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
    bool left_cam = GetRandomValue(0, 100) % 2;

    float x, y, w, h;
    w = 100;
    h = 100;

    if (left_cam) {
      x = camera_left_x - GetRandomValue(10, 40) - w;
      y = GetRandomValue(camera_bottom_y + 40, camera_top_y - 40);
    } else {
      x = camera_right_x + GetRandomValue(10, 40);
      y = GetRandomValue(camera_bottom_y + 40, camera_top_y - 40);
    }

    auto &e = owner->scene->create();
    e.add<c_Transform>(Vector2{x, y}, Vector2{w, h});
    e.add<RectangleRenderer>(Rectangle{x, y, w, h}, GREEN);
    e.add<EnemyController>(200.0f, p->get<c_Transform>());
    e.add<Health>(1);
  }
};

Rectangle rectOf(GameObject *go) {
  auto *t = go->get<c_Transform>();
  return {t->position.x, t->position.y, t->scale.x, t->scale.y};
};

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
        owner->scene->destroy(b);
        if (hp->value <= 0)
          owner->scene->destroy(e);
        break;
      }
    }
  };
}
