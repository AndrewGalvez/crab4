#pragma once
#include "component.hpp"
#include <functional>

class Button : public Component {
public:
  void update(float dt) override;
  std::function<void(void)> onClicked = []() {};
  Button(std::function<void(void)> onClicked) : onClicked(onClicked) {};
  bool hovered = false;
};
