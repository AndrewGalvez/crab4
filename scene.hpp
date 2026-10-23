#pragma once
#include "assets.hpp"
#include "game_object.hpp"
#include <memory>
#include <vector>

class c_Camera;

class Game;

class Scene {
private:
  std::vector<std::unique_ptr<GameObject>> contents;
  std::vector<std::unique_ptr<GameObject>> pendingAdditions;
  std::vector<GameObject *> pendingDestructions;
  c_Camera *activeCamera = nullptr;

public:
  Game *game = nullptr;
  GameObject &create();
  void destroy(GameObject *go);
  void setActiveCamera(c_Camera *cam) { activeCamera = cam; };
  const c_Camera *getActiveCamera() { return activeCamera; }
  AssetManager asset_manager;

  void draw();
  void update(float dt);

  template <typename T> std::vector<GameObject *> findAllWithComponent() {
    std::vector<GameObject *> res;
    for (auto &go : contents)
      if (go->get<T>())
        res.push_back(go.get());
    return res;
  }

  template <typename T> GameObject *findFirstWithComponent() {
    GameObject *res = nullptr;
    for (auto &go : contents)
      if (go->get<T>()) {
        res = go.get();
        return res;
      }
    return res;
  }
};
