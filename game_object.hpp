#pragma once
#include "component.hpp"
#include <memory>
#include <vector>

class Scene;

class GameObject {
private:
  std::vector<std::unique_ptr<Component>> components;

public:
  template <typename T, typename... Args> T &add(Args &&...args) {
    auto c = std::make_unique<T>(std::forward<Args>(args)...);
    T &ref = *c;
    c->owner = this;
    components.push_back(std::move(c));
    return ref;
  }

  template <typename T> T *get() {
    for (auto &c : components) {
      if (auto *p = dynamic_cast<T *>(c.get()))
        return p;
    }
    return nullptr;
  }

  Scene *scene = nullptr;
  bool destroyed = false;

  void draw();
  void update(float dt);
  void drawUI();
};
