#pragma once
#include "game_object.hpp"
#include "physics.hpp"
#include "raylib.h"

inline Rectangle rectOf(GameObject *go) {
  auto *t = go->get<c_Transform>();
  return {t->position.x, t->position.y, t->scale.x, t->scale.y};
};
