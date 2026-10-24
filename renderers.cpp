#include "renderers.hpp"
#include "camera.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "physics.hpp"
#include "scene.hpp"
#include <cmath>
#include <raylib.h>

void RectangleRenderer::draw() { DrawRectangleRec(rect, color); }

void RectangleRendererUI::drawUI() { DrawRectangleRec(rect, color); }

void RectangleRenderer::update(float dt) {
  color.a = (unsigned char)opacity;
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;
  rect.x = t->position.x;
  rect.y = t->position.y;
  rect.width = t->scale.x;
  rect.height = t->scale.y;
}

void CircleRenderer::draw() { DrawCircle(center.x, center.y, radius, color); }

void CircleRenderer::update(float dt) {
  color.a = (unsigned char)opacity;
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;
  center.x = t->position.x + t->scale.x / 2;
  center.y = t->position.y + t->scale.y / 2;
};

void PlayerMagnetCircleDisplayRenderer::update(float dt) {
  color.a = (unsigned char)opacity;
  auto *t = owner->get<c_Transform>();
  if (!t)
    return;
  center.x = t->position.x + t->scale.x / 2;
  center.y = t->position.y + t->scale.y / 2;
  radius = owner->scene->game->state.pstats.magnetRadius;
}

void PlayerMagnetCircleDisplayRenderer::draw() {
  DrawCircleGradient(center, radius, color, ColorAlpha(color, 0));
  DrawCircleGradient(center, radius + 20, color, ColorAlpha(color, 0));
};

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

void BackgroundRenderer::draw() {
  auto cams = owner->scene->findAllWithComponent<c_Camera>();
  if (cams.empty())
    return;
  auto *camComp = cams[0]->get<c_Camera>();
  const Camera2D &cam = camComp->raw();

  // World-space corners of what's currently on screen
  Vector2 tl = GetScreenToWorld2D({0, 0}, cam);
  Vector2 br = GetScreenToWorld2D({(float)800, (float)600}, cam);

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

void UITextRenderer::drawUI() {
  DrawText(value.c_str(), pos.x, pos.y, fontSize, color);
};

void ImageRenderer::update(float dt) {
  frame++;
  if (maxFrame < frame) {
    frame = 0;
    animFrame++;
    if (animFrameMax < animFrame)
      animFrame = 0;
  }

  getSourceFromFrame(*this);

  auto *t = owner->get<c_Transform>();
  if (!t)
    return;

  dest.x = t->position.x;
  dest.y = t->position.y;
  dest.width = t->scale.x;
  dest.height = t->scale.y;
};

void ImageRenderer::draw() {
  DrawTexturePro(*owner->scene->asset_manager.texture(file), source, dest,
                 origin, rotation, WHITE);
};

void ImageRendererUI::update(float dt) {
  frame++;
  if (maxFrame < frame) {
    frame = 0;
    animFrame++;
    if (animFrameMax < animFrame)
      animFrame = 0;
  }

  getSourceFromFrame(*this);

  auto *t = owner->get<c_Transform>();
  if (!t)
    return;

  dest.x = t->position.x;
  dest.y = t->position.y;
  dest.width = t->scale.x;
  dest.height = t->scale.y;
};

void ImageRendererUI::drawUI() {
  DrawTexturePro(*owner->scene->asset_manager.texture(file), source, dest,
                 origin, rotation, WHITE);
};
