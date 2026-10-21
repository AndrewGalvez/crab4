#pragma once
#include "component.hpp"

class Lifetime : public Component {
public:
  int frames_left = 0;
  int frames_start = 0;
  Lifetime(int frames_left)
      : frames_left(frames_left), frames_start(frames_left) {};
  void update(float dt) override;
};

class Bullet : public Component {
public:
  int damage;
  Bullet(int damage = 1) : damage(damage) {}
};

class Health : public Component {
public:
  int value;
  Health(int value = 1) : value(value) {}
};
