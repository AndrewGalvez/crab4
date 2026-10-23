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
