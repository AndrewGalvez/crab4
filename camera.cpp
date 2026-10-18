#include "camera.hpp"
#include "components.hpp"
#include "game_object.hpp"
#include <cmath>

c_Camera::c_Camera() {
  camera.zoom = 1.0f;
  camera.rotation = 0.0f;
}

void c_Camera::update(float dt) {
  auto *self = owner->get<c_Transform>();
  if (!self)
    return;

  if (target && followSpeed > 0.0f) {
    if (auto *tt = target->get<c_Transform>()) {
      float t = 1.0f - std::exp(-followSpeed * dt);
      self->position.x += (tt->position.x - self->position.x) * t;
      self->position.y += (tt->position.y - self->position.y) * t;
    }
  }

  camera.target = {self->position.x + targetOffset.x,
                   self->position.y + targetOffset.y};
  camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
}

void c_Camera::begin() { BeginMode2D(camera); }
void c_Camera::end() { EndMode2D(); }

void c_Camera::setZoom() { camera.zoom = this->zoom; };
