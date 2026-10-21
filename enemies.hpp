#pragma once
#include "component.hpp"
#include "physics.hpp"

class EnemyController : public Component {
public:
  float speed;
  c_Transform *target;
  EnemyController(float speed, c_Transform *target)
      : speed(speed), target(target) {};
  void update(float dt) override;
};
