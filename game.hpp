#pragma once
#include "player.hpp"
#include "scene.hpp"
#include <functional>

struct GameState {
  PlayerStats pstats;
};

class Game {
private:
  using SceneBuilder = std::function<void(Scene &)>;
  std::unique_ptr<Scene> scene;
  std::unordered_map<std::string, SceneBuilder> scenebuilders;
  std::string pendingScene;

  void load(const std::string &name);

public:
  Game();
  void update(float dt);
  void draw();
  void addScene(const std::string &name, SceneBuilder b);
  void switchTo(const std::string &name);

  GameState state;
};
