#pragma once
#include "component.hpp"
#include "raylib.h"
#include <functional>
#include <raymath.h>
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

class CircleRenderer : public Renderer {
public:
  float radius;
  Color color;
  int opacity = 255;
  Vector2 center;
  CircleRenderer(Vector2 center, float radius, Color color)
      : center(center), radius(radius), color(color) {};
  CircleRenderer() {};

  void draw() override;
  void update(float dt) override;
};

class PlayerMagnetCircleDisplayRenderer : public CircleRenderer {
public:
  PlayerMagnetCircleDisplayRenderer(Vector2 center, float radius, Color color)
      : CircleRenderer(center, radius, color) {
    opacity = 100;
  };
  void update(float dt) override;
  void draw() override;
};

class UITextRenderer : public Renderer {
public:
  std::string value;
  Color color;
  int fontSize;
  Vector2 pos;

  UITextRenderer(std::string v, Color c, int fs, Vector2 p)
      : value(v), color(c), fontSize(fs), pos(p) {};

  void drawUI() override;
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

class ImageRenderer : public Renderer {
public:
  std::string file;
  Rectangle dest;
  Rectangle source;
  int frame;
  int maxFrame;
  int animFrame;
  int animFrameMax;
  Vector2 origin;
  float rotation;
  std::function<void(ImageRenderer &)> getSourceFromFrame = nullptr;

  ImageRenderer(std::string file, Rectangle dest, Rectangle source, int frame,
                int maxFrame, Vector2 origin, float rotation,
                std::function<void(ImageRenderer &)> getSourceFromFrame,
                int animFrame, int animFrameMax)
      : file(file), dest(dest), source(source), frame(frame),
        maxFrame(maxFrame), origin(origin), rotation(rotation),
        getSourceFromFrame(getSourceFromFrame), animFrame(animFrame),
        animFrameMax(animFrameMax) {}

  void update(float dt) override;
  void draw() override;
};
