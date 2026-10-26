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
#include <raylib.h>
#include <string>

void Game::addScene(const std::string &name, SceneBuilder b) {
  scenebuilders[name] = std::move(b);
}

void Game::switchTo(const std::string &name) {
  pendingScene = name;
  transitioning = true;
}

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
        .loadFromStrings({"##########", "#........#", "#........#",
                          "#........#", "#........#", "#........#",
                          "#........#", "#........#", "#........#",
                          "##########"});
    scene.create()
        ._add<Door>("game", Vector2{0, 100})
        ._add<RectangleRenderer>(Rectangle{-100, -500, 200, 100}, YELLOW)
        ._add<c_Transform>(Vector2{-100, -500}, Vector2{200, 100});
    scene.create()
        ._add<Door>("shop", Vector2{-50, -899})
        ._add<ImageRenderer>(
            "assets/door.png", Rectangle{-100, 400, 200, 100},
            Rectangle{0, 0, 32, 16}, 0, 0, Vector2{0, 0}, 0.0f,
            [](ImageRenderer &self) {}, 0, 0)
        ._add<c_Transform>(Vector2{-100, 400}, Vector2{200, 100});

    scene.create().add<CollisionManager>();
  });
  addScene("shop", [this](Scene &scene) {
    prefabs::player(scene, &state.pstats, state.spawnPos);
    prefabs::ui(scene, scene.asset_manager, state.pstats);

    auto &cam = scene.create();
    cam.add<c_Transform>(Vector2{0, 0}, Vector2{1, 1});
    auto &camraw = cam.add<c_Camera>();
    camraw.followSpeed = 0.0f;
    camraw.zoom = 0.3f;
    camraw.targetOffset = {0, 0};
    camraw.setZoom();
    scene.setActiveCamera(&camraw);

    scene.create()
        .add<TileMap>(100.0f, Vector2{-1350.0f, -1300.0f})
        .loadFromFile("maps/shop");

    scene.create().add<CollisionManager>();
    scene.create().add<PhysicsManager>();
    scene.create()
        ._add<Door>("room", Vector2{-50, 299})
        ._add<ImageRenderer>(
            "assets/door.png", Rectangle{-100, -1000, 200, 100},
            Rectangle{0, 0, 32, 16}, 0, 0, Vector2{0, 0}, 0.0f,
            [](ImageRenderer &self) {}, 0, 0)
        ._add<c_Transform>(Vector2{-100, -1000}, Vector2{200, 100});

    scene.create()
        ._add<ImageRenderer>(
            "assets/shopkeep1.png", Rectangle{-950, -900, 500, 250},
            Rectangle{0, 0, 32, 16}, 0, 0, Vector2{0, 0}, 0.0f,
            [](ImageRenderer &self) {}, 0, 0)
        ._add<c_Transform>(Vector2{-950, -900}, Vector2{500, 250})
        ._add<Door>("shopkeep1", Vector2{0, 0});
  });

  addScene("shopkeep1", [this](Scene &scene) {
    scene.create()
        ._add<ID>("shopkeep1dialogue")
        .add<UITextRenderer>("hello i am jimbler\n buy my stuff", WHITE, 32,
                             Vector2{0, 0});
    scene.create().add<ImageRendererUI>(
        "assets/waltuh.png", Rectangle{0, 100, 250, 500},
        Rectangle{0, 0, 240, 330}, 0, 0, Vector2{0, 0}, 0.0f,
        [](ImageRendererUI &self) {}, 0, 0);
    auto &lpi = scene.create()
                    ._add<ImageRendererUI>(
                        "assets/luckpotion.png", Rectangle{350, 100, 100, 100},
                        Rectangle{0, 0, 8, 8}, 0, 0, Vector2{0, 0}, 0.0f,
                        [](ImageRendererUI &self) {}, 0, 0)
                    ._add<c_Transform>(Vector2{350, 100}, Vector2{100, 100});

    lpi.add<HoverListener>(
        [](HoverListener &self) {
          self.owner->scene->findFirstWithID("shopkeep1dialogue")
              ->get<UITextRenderer>()
              ->value =
              "mmm yes, this makes you\n twice as lucky for one minute";
          self.owner->scene->asset_manager.playSound("assets/tick.wav");
        },
        [](HoverListener &self) {}, [](HoverListener &self) {},
        lpi.get<c_Transform>());
    scene.create().add<UITextRenderer>(
        "Luck Potion", WHITE, 24,
        textPositionCenteredInRect({350, 215, 100, 1}, "Luck Potion", 24));
    scene.create()
        ._add<ImageRendererUI>(
            "assets/gold.png", Rectangle{260, 500, 50, 50},
            Rectangle{0, 0, 16, 16}, 0, 10, Vector2{0, 0}, 0.0f,
            [](ImageRendererUI &self) {
              self.source = Rectangle{16.0f * self.animFrame, 0, 16, 16};
            },
            0, 8)
        .add<UITextRenderer>(
            "0", WHITE, 50, Vector2{325, 505},
            [&stats = this->state.pstats](UITextRenderer &self) {
              self.value = std::to_string(stats.gold);
            });
  });

  load("shopkeep1");
}

void Game::update(float dt) {
  if (transitioning) {
    if (!pendingScene.empty()) {
      transitionProgress += 4;
      if (transitionProgress == 100) {
        std::string name = std::move(pendingScene);
        pendingScene.clear();
        load(name);
        scene->update(0);
        scene->update(0);
        scene->update(0);
      }
    } else {
      transitionProgress--;
      if (transitionProgress == 0)
        transitioning = false;
    }
    if (transitionProgress % 50 == 0 && transitionProgress != 0)
      scene->asset_manager.playSound(
          transitionProgress == 50 ? "assets/hit1.wav" : "assets/hit2.wav");

    return;
  }

  if (scene)
    scene->update(dt);
}

void Game::draw() {
  if (scene)
    scene->draw();
  if (transitioning)
    DrawRectangle(
        0, 0, 800, 600,
        ColorAlpha(DARKGREEN, easeInOut(transitionProgress / 100.0f)));
}
