#include "components.hpp"
#include "component.hpp"
#include "game_object.hpp"
#include "scene.hpp"
#include <raylib.h>
#include <raymath.h>

void Lifetime::update(float dt) {
  frames_left--;
  if (frames_left <= 0)
    this->owner->scene->destroy(this->owner);
};
