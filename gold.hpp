#pragma once
#include "component.hpp"

class Gold : public Component {
public:
  void update(float dt) override;
  float speed = 500.0f;
};
