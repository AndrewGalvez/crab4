#pragma once
#include "component.hpp"
#include "raylib.h"

class c_Transform : public Component {
public:
  Vector2 position = {0, 0};
  Vector2 scale = {0, 0};
  Vector2 _pos = {0, 0};
  explicit c_Transform(Vector2 p, Vector2 scale)
      : position(p), scale(scale), _pos(p) {}
};

class Velocity : public Component {
public:
  Vector2 value = {0, 0};
  Velocity(Vector2 value) : value(value) {};
};

class Friction : public Component {
public:
  int value;
  Friction(int value = 0) : value(value) {}
};

class PhysicsManager : public Component {
public:
  void update(float dt) override;
};
