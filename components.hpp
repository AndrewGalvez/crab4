#pragma once
#include "component.hpp"
#include "raylib.h"
#include <string>

class c_Transform : public Component {
public:
  Vector2 position = {0, 0};
  explicit c_Transform(Vector2 p = {0, 0}) : position(p) {}
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
