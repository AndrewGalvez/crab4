#include "components.hpp"
#include "camera.hpp"
#include "component.hpp"
#include "game_object.hpp"
#include "scene.hpp"
#include <cmath>
#include <raylib.h>
#include <string>

void RectangleRenderer::draw() { DrawRectangleRec(rect, color); }

void RectangleRenderer::update(float dt) {
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;
  rect.x = t->position.x;
  rect.y = t->position.y;
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
  if (IsKeyDown(KEY_A))
    t->position.x -= speed * dt;
  if (IsKeyDown(KEY_W))
    t->position.y -= speed * dt;
  if (IsKeyDown(KEY_S))
    t->position.y += speed * dt;
  if (IsKeyDown(KEY_D))
    t->position.x += speed * dt;
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
