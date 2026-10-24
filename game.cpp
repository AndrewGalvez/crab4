#include "game.hpp"
#include "button.hpp"
#include "camera.hpp"
#include "components.hpp"
#include "managers.hpp"
#include "physics.hpp"
#include "player.hpp"
#include "prefabs.hpp"
#include "renderers.hpp"
#include "scene.hpp"
#include "tilemap.hpp"
#include "utils.hpp"
#include <string>

void Game::addScene(const std::string &name, SceneBuilder b) {
  scenebuilders[name] = std::move(b);
}

void Game::switchTo(const std::string &name) { pendingScene = name; }

void Game::load(const std::string &name) {
  auto it = scenebuilders.find(name);
  if (it == scenebuilders.end())
    return;

  scene = std::make_unique<Scene>();
  scene->game = this;
  it->second(*scene);
}

Game::Game() {
  addScene("mainmenu", [](Scene &scene) {});
  addScene("deadmenu", [this](Scene &scene) {
    scene.create().add<UITextRenderer>(
        "You Died", WHITE, 64,
        textPositionCenteredInRect({300, 100, 200, 100}, "You Died", 64));
    scene.create()
        ._add<RectangleRenderer>(Rectangle{300, 250, 200, 100}, GRAY)
        ._add<UITextRenderer>(
            "restart", BLACK, 32,
            textPositionCenteredInRect({300, 250, 200, 100}, "restart", 32))
        .add<Button>([this]() {
          this->state.pstats.reset();
          this->switchTo("game");
        });
  });
  addScene("game", [this](Scene &scene) {
    // background -----------------------------------
    auto &background = scene.create();
    background.add<BackgroundRenderer>(WHITE, BLACK, Vector2{200, 200});

    // player ---------------------------------------
    auto &player = prefabs::player(scene, &state.pstats, state.spawnPos);
    // UI
    prefabs::ui(scene, scene.asset_manager, state.pstats);

    // game manager -------------------------------
    auto &gameManager = scene.create();
    gameManager.add<GameManager>();
    // collision manager --------------------------
    auto &collisionManager = scene.create();
    collisionManager.add<CollisionManager>();
    // camera ---------------------------------------
    auto &cam = scene.create();
    cam.add<c_Transform>(Vector2{0, 0}, Vector2{1, 1});
    auto &camraw = cam.add<c_Camera>();
    camraw.target = &player;
    camraw.targetOffset = {50, 50};
    camraw.followSpeed = 5.0f;
    camraw.zoom = 0.4f;
    camraw.setZoom();
    scene.setActiveCamera(&camraw);
  });
  addScene("room", [this](Scene &scene) {
    auto &player = prefabs::player(scene, &state.pstats, state.spawnPos);
    prefabs::ui(scene, scene.asset_manager, state.pstats);

    auto &cam = scene.create();
    cam.add<c_Transform>(Vector2{0, 0}, Vector2{1, 1});
    auto &camraw = cam.add<c_Camera>();
    camraw.target = &player;
    camraw.targetOffset = {50, 50};
    camraw.followSpeed = 5.0f;
    camraw.zoom = 0.4f;
    camraw.setZoom();
    scene.setActiveCamera(&camraw);

    scene.create()
        .add<TileMap>(100.0f, Vector2{-500.0f, -500.0f})
        .loadFromStrings({"####..####", "#........#", "#........#",
                          "#........#", "#........#", "#........#",
                          "#........#", "#........#", "#........#",
                          "##########"});
    scene.create()
        ._add<Door>("game", Vector2{0, 100})
        ._add<RectangleRenderer>(Rectangle{-100, -500, 200, 100}, YELLOW)
        ._add<c_Transform>(Vector2{-100, -500}, Vector2{200, 100});

    scene.create().add<CollisionManager>();
  });

  load("room");
}

void Game::update(float dt) {
  if (!pendingScene.empty()) {
    std::string name = std::move(pendingScene);
    pendingScene.clear();
    load(name);
    scene->update(0);
  }

  if (scene)
    scene->update(dt);
}

void Game::draw() {
  if (scene)
    scene->draw();
}
