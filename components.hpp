#pragma once
#include "component.hpp"
#include "physics.hpp"
#include <functional>
#include <raylib.h>
#include <string>

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

class Wall : public Component {};

class Solid : public Component {};

class Door : public Component {
public:
  Vector2 arrivesAt;
  std::string dest;

  Door(std::string dest, Vector2 pso) : arrivesAt(pso), dest(dest) {}
};

class Enabled : public Component {
public:
  bool value = true;
  Enabled() {}
  Enabled(bool value) : value(value) {}
};

class CollisionListener : public Component {
public:
  std::function<void(CollisionListener &l)> onCollisionStart;
  std::function<void(CollisionListener &l)> onCollisionStop;
  std::function<void(CollisionListener &l)> whileColliding;
  c_Transform *me = nullptr;
  c_Transform *watch = nullptr;
  bool colliding = false;

  CollisionListener(std::function<void(CollisionListener &l)> onCollisionStart,
                    std::function<void(CollisionListener &l)> onCollisionStop,
                    std::function<void(CollisionListener &l)> whileColliding,
                    c_Transform *t, c_Transform *watch)
      : onCollisionStart(onCollisionStart), onCollisionStop(onCollisionStop),
        whileColliding(whileColliding), me(t), watch(watch) {}

  void update(float dt) override;
};

class ID : public Component {
public:
  std::string val;
  ID(std::string val) : val(val) {}
};
