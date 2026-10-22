#include "game.hpp"
#include "camera.hpp"
#include "managers.hpp"
#include "physics.hpp"
#include "player.hpp"
#include "renderers.hpp"
#include "scene.hpp"

Game::Game() {
  // background -----------------------------------
  auto &background = scene.create();
  background.add<BackgroundRenderer>(WHITE, BLACK, Vector2{200, 200});

  // player ---------------------------------------
  auto &player = scene.create();
  player.add<c_Transform>((Vector2){0, 0}, (Vector2){100, 100});
  player.add<RectangleRenderer>((Rectangle){0, 0, 100, 100}, RED);
  player.add<UIPositionLabel>();
  auto pstats = PlayerStats();
  player.add<PlayerController>(pstats);
  player.add<PlayerHealthbar>(&player.get<PlayerController>()->stats.health,
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
}

void Game::update(float dt) { scene.update(dt); }

void Game::draw() {
  scene.draw();
  DrawFPS(0, 0);
}
