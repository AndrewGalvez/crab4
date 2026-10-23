#include "button.hpp"
#include "game_object.hpp"
#include "renderers.hpp"
#include <raylib.h>

void Button::update(float dt) {
  auto *rr = owner->get<RectangleRenderer>();

  if (!rr)
    return;

  Vector2 mp = GetMousePosition();
  if (CheckCollisionPointRec(mp, rr->rect)) {
    if (!hovered) {
      rr->color = ColorBrightness(rr->color, -0.1f);
      hovered = true;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      this->onClicked();
    };
  } else if (hovered) {
    rr->color = ColorBrightness(rr->color, 0.1f), hovered = false;
  }
};
