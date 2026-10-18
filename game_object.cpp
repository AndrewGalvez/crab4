#include "game_object.hpp"

void GameObject::update(float dt) {
  for (auto &c : components)
    c->update(dt);
}

void GameObject::draw() {
  for (auto &c : components)
    c->draw();
}

void GameObject::drawUI() {
  for (auto &c : components)
    c->drawUI();
}
