#include "scene.hpp"
#include <algorithm>

#include "camera.hpp"
#include <vector>

GameObject &Scene::create() {
  pendingAdditions.push_back(std::make_unique<GameObject>());
  pendingAdditions.back()->scene = this;
  return *pendingAdditions.back();
}

void Scene::destroy(GameObject *go) {
  go->destroyed = true;
  pendingDestructions.push_back(go);
}

void Scene::draw() {
  if (activeCamera)
    activeCamera->begin();
  for (auto &go : contents)
    go->draw();
  if (activeCamera)
    activeCamera->end();

  for (auto &go : contents)
    go->drawUI();
};

void Scene::update(float dt) {
  for (auto &go : contents)
    if (!go->destroyed)
      go->update(dt);

  if (!pendingDestructions.empty()) {
    auto it = std::stable_partition(
        contents.begin(), contents.end(), [this](const auto &go) {
          return std::find(pendingDestructions.begin(),
                           pendingDestructions.end(),
                           go.get()) == pendingDestructions.end();
        });
    contents.erase(it, contents.end());
    pendingDestructions.clear();
  };

  if (!pendingAdditions.empty()) {
    for (auto &go : pendingAdditions) {
      contents.push_back(std::move(go));
    }
    pendingAdditions.clear();
  }
}
