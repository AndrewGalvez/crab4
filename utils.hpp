#pragma once
#include "game_object.hpp"
#include "physics.hpp"
#include "raylib.h"
#include <string>

inline Rectangle rectOf(GameObject *go) {
  auto *t = go->get<c_Transform>();
  return {t->position.x, t->position.y, t->scale.x, t->scale.y};
};

inline Vector2 rectCenter(Rectangle r) {
  return {r.x + r.width / 2, r.y + r.height / 2};
}

inline Vector2 textPositionCenteredInRect(Rectangle r, std::string text,
                                          float fs) {
  Vector2 c = rectCenter(r);
  float x = c.x - MeasureText(text.c_str(), fs) / 2.0f;
  float y = c.y - fs / 2.0f;

  return {x, y};
};

inline bool resolveRectVsStatic(Rectangle &mover, Rectangle wall,
                                Vector2 &push) {
  if (!CheckCollisionRecs(mover, wall))
    return false;

  Rectangle o = GetCollisionRec(mover, wall);
  Vector2 mc = rectCenter(mover);
  Vector2 wc = rectCenter(wall);

  if (o.width < o.height)
    push = {mc.x < wc.x ? -o.width : o.width, 0};
  else
    push = {0, mc.y < wc.y ? -o.height : o.height};
  return true;
}
