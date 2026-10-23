#include "game.hpp"
#include "button.hpp"
#include "camera.hpp"
#include "managers.hpp"
#include "physics.hpp"
#include "player.hpp"
#include "renderers.hpp"
#include "scene.hpp"
#include "utils.hpp"

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
    auto &player = scene.create();
    player.add<c_Transform>((Vector2){0, 0}, (Vector2){100, 100});
    player.add<RectangleRenderer>((Rectangle){0, 0, 100, 100}, RED);
    player.add<UIPositionLabel>();
    player.add<PlayerController>(this->state.pstats);
    player.add<PlayerHealthbar>(
        &player.get<PlayerController>()->stats.health,
        &player.get<PlayerController>()->stats.maxHealth);
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

  load("deadmenu");
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
