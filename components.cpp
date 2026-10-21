#include "components.hpp"
#include "camera.hpp"
#include "component.hpp"
#include "game_object.hpp"
#include "player.hpp"
#include "renderers.hpp"
#include "scene.hpp"
#include <cmath>
#include <raylib.h>
#include <raymath.h>

void Lifetime::update(float dt) {
  frames_left--;
  if (frames_left <= 0)
    this->owner->scene->destroy(this->owner);
};
