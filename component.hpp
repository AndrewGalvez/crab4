#pragma once

class GameObject;

class Component {
public:
  GameObject *owner = nullptr;

  virtual ~Component() = default;
  virtual void update(float dt);
  virtual void draw();
  virtual void drawUI();
};
