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
    static_assert(std::is_base_of_v<Component, T>,
                  "add<T>: T must derive from Component");
    auto c = std::make_unique<T>(std::forward<Args>(args)...);
    T &ref = *c;
    c->owner = this;
    components.push_back(std::move(c));
    return ref;
  }

  template <typename T, typename... Args> GameObject &_add(Args &&...args) {
    add<T>(std::forward<Args>(args)...);
    return *this;
  }

  template <typename T> T *get() {
    static_assert(std::is_base_of_v<Component, T>,
                  "get<T>: T must derive from Component");
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
