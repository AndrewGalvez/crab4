#include "game.hpp"
#include "button.hpp"
#include "camera.hpp"
#include "managers.hpp"
#include "physics.hpp"
#include "player.hpp"
#include "renderers.hpp"
#include "scene.hpp"
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
    auto &player = scene.create();
    player.add<c_Transform>((Vector2){0, 0}, (Vector2){100, 100});
    player.add<PlayerMagnetCircleDisplayRenderer>(Vector2{0, 0}, 5000.0f,
                                                  ColorAlpha(BLUE, 100));
    player.add<RectangleRenderer>((Rectangle){0, 0, 100, 100}, RED);
    player.add<UIPositionLabel>();
    player.add<PlayerController>(&this->state.pstats);
    player.add<Velocity>(Vector2{0.0f, 0.0f});
    player.add<Friction>(400.0f);
    player.add<PhysicsManager>();
    // UI
    scene
        .create()

        ._add<RectangleRendererUI>(Rectangle{275, 450, 250, 150},
                                   ColorAlpha(GRAY, 0.8))
        ._add<ImageRendererUI>(
            "assets/gold.png", Rectangle{280, 460, 50, 50},
            Rectangle{0, 0, 16, 16}, 0, 10, Vector2{0, 0}, 0.0f,
            [](ImageRendererUI &self) {
              self.source = Rectangle{16.0f * self.animFrame, 0, 16, 16};
            },
            0, 8)
        ._add<PlayerHealthbar>(&state.pstats.health, &state.pstats.maxHealth)
        ._add<UITextRenderer>("300", WHITE, 50, Vector2{340, 465},
                              [&stats = state.pstats](UITextRenderer &self) {
                                self.value = std::to_string(stats.gold);
                              });

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
