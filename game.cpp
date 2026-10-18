#include "game.hpp"
#include "camera.hpp"
#include "components.hpp"
#include "scene.hpp"

Game::Game() {
  // background -----------------------------------
  auto &background = scene.create();
  background.add<BackgroundRenderer>(WHITE, BLACK, Vector2{200, 200});

  // player ---------------------------------------
  auto &player = scene.create();
  player.add<c_Transform>((Vector2){0, 0});
  player.add<RectangleRenderer>((Rectangle){0, 0, 100, 100}, RED);
  player.add<UIPositionLabel>();
  player.add<PlayerController>(1000);
  // camera ---------------------------------------
  auto &cam = scene.create();
  cam.add<c_Transform>(Vector2{0, 0});
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
