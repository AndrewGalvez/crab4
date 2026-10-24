#pragma once
#include "component.hpp"

class PlayerStats {
public:
  float speed;
  float _speed;
  float health;
  float _health;
  float maxHealth;
  float _maxHealth;
  float bulletDamage;
  float _bulletDamage;
  float bulletSpeed;
  float _bulletSpeed;
  float magnetRadius;
  float _magnetRadius;
  int gold;

  PlayerStats(float speed = 500.0f, float health = 100.0f,
              float maxHealth = 100.0f, float bulletDamage = 25.0f,
              float bulletSpeed = 2000.0f, float magnetRadius = 500.0f,
              int gold = 0)
      : speed(speed), health(health), maxHealth(maxHealth),
        bulletDamage(bulletDamage), bulletSpeed(bulletSpeed), _speed(speed),
        _health(health), _maxHealth(maxHealth), _bulletDamage(bulletDamage),
        _bulletSpeed(bulletSpeed), magnetRadius(magnetRadius),
        _magnetRadius(magnetRadius) {}

  void reset() {
    speed = _speed;
    health = _health;
    maxHealth = _maxHealth;
    bulletDamage = _bulletDamage;
    bulletSpeed = _bulletSpeed;
    magnetRadius = _magnetRadius;
    gold = 0;
  }
};

class PlayerController : public Component {
public:
  PlayerStats *stats;
  PlayerController(PlayerStats *stats) : stats(stats) {};
  void update(float dt) override;
};

class PlayerHealthbar : public Component {
public:
  float *health;
  float *maxHealth;
  void drawUI() override;
  PlayerHealthbar(float *health, float *maxhealth)
      : health(health), maxHealth(maxhealth) {}
};
