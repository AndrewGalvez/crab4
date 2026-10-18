#pragma once
#include "scene.hpp"

class Game {
private:
  Scene scene;

public:
  Game();
  void update(float dt);
  void draw();
};
