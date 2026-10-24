#pragma once
#include "component.hpp"

class Gold : public Component {
public:
  void update(float dt) override;
  float speed = 1000.0f;
};
