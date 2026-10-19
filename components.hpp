#pragma once
#include "component.hpp"
#include "raylib.h"
#include <string>

class c_Transform : public Component {
public:
  Vector2 position = {0, 0};
  Vector2 scale = {0, 0};
  explicit c_Transform(Vector2 p, Vector2 scale) : position(p), scale(scale) {}
};

class Renderer : public Component {};

class RectangleRenderer : public Renderer {
public:
  Rectangle rect;
  Color color;
  RectangleRenderer(Rectangle rect, Color color) : rect(rect), color(color) {};

  void draw() override;
  void update(float dt) override;
};

class UITextRenderer : public Renderer {
public:
  std::string value;
  Color color;
  int fontSize;
  Vector2 pos;

  UITextRenderer(std::string v, Color c, int fs, Vector2 p)
      : value(v), color(c), fontSize(fs), pos(p) {};
};

class UIPositionLabel : public UITextRenderer {
public:
  UIPositionLabel();

  void drawUI() override;
  void update(float dt) override;
};

class PlayerController : public Component {
public:
  float speed;
  PlayerController(float speed) : speed(speed) {};
  void update(float dt) override;
};

class EnemyController : public Component {
public:
  float speed;
  c_Transform *target;
  EnemyController(float speed, c_Transform *target)
      : speed(speed), target(target) {};
  void update(float dt) override;
};

class GameManager : public Component {
public:
  int frame = 0;
  void update(float dt) override;
};

// tiles
class BackgroundRenderer : public Renderer {
public:
  Color color_a;
  Color color_b;
  Vector2 tilesize;
  explicit BackgroundRenderer(Color a, Color b, Vector2 t)
      : color_a(a), color_b(b), tilesize(t) {};
  void draw() override;
};

class Velocity : public Component {
public:
  Vector2 value = {0, 0};
  Velocity(Vector2 value) : value(value) {};
};

class Lifetime : public Component {
public:
  int frames_left = 0;
  Lifetime(int frames_left) : frames_left(frames_left) {};
  void update(float dt) override;
};

class PhysicsManager : public Component {
public:
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

class CollisionManager : public Component {
public:
  void update(float dt) override;
};
