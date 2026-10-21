#pragma once
#include "component.hpp"
#include "raylib.h"
#include <string>

class Renderer : public Component {};

class RectangleRenderer : public Renderer {
public:
  Rectangle rect;
  Color color;
  int opacity = 255;
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
