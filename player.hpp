#pragma once
#include "component.hpp"

struct PlayerStats {
  float speed;
  float health;
  float maxHealth;
  float bulletDamage;
  float bulletSpeed;

  PlayerStats(float speed = 500.0f, float health = 100.0f,
              float maxHealth = 100.0f, float bulletDamage = 25.0f,
              float bulletSpeed = 2000.0f)
      : speed(speed), health(health), maxHealth(maxHealth),
        bulletDamage(bulletDamage), bulletSpeed(bulletSpeed) {}
};

class PlayerController : public Component {
public:
  PlayerStats stats;
  PlayerController(PlayerStats stats) : stats(stats) {};
  void update(float dt) override;
};
