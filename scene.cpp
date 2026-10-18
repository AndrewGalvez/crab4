#include "scene.hpp"
#include "camera.hpp"

GameObject &Scene::create() {
  contents.push_back(std::make_unique<GameObject>());
  contents.back()->scene = this;
  return *contents.back();
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
    go->update(dt);
}
