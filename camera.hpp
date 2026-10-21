#pragma once
#include "game_object.hpp"
#include "raylib.h"

class c_Camera : public Component {
public:
  GameObject *target = nullptr;
  Vector2 targetOffset{0, 0};
  float followSpeed = 6.0f;
  float zoom = 1.0f;

  c_Camera();

  void update(float dt) override;
  void setZoom();

  void begin();
  void end();

  const Camera2D &raw() const { return camera; }

  Camera2D camera{};
};
