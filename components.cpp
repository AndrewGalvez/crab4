#include "components.hpp"
#include "component.hpp"
#include "game_object.hpp"
#include "scene.hpp"
#include "utils.hpp"
#include <raylib.h>
#include <raymath.h>

void Lifetime::update(float dt) {
  frames_left--;
  if (frames_left <= 0)
    this->owner->scene->destroy(this->owner);
};

void CollisionListener::update(float dt) {
  if (CheckCollisionRecs(rectOf(me->owner), rectOf(watch->owner))) {
    if (!colliding) {
      onCollisionStart(*this);
      colliding = true;
    }
    whileColliding(*this);
  } else {
    if (colliding) {
      onCollisionStop(*this);
      colliding = false;
    }
  }
};

void HoverListener::update(float dt) {
  if (CheckCollisionPointRec(GetMousePosition(), rectOf(me->owner))) {
    if (!hovering) {
      onHoverStart(*this);
      hovering = true;
    }
    whileColliding(*this);
  } else {
    if (hovering) {
      onHoverStop(*this);
      hovering = false;
    }
  }
};
