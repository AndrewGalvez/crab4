#pragma once
#include "component.hpp"

class CollisionManager : public Component {
public:
  void update(float dt) override;
};

class GameManager : public Component {
public:
  int frame = 0;
  void update(float dt) override;
};
