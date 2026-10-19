#include "game.hpp"
#include "raylib.h"

int main() {
  InitWindow(800, 600, "crab 4");
  SetTargetFPS(60);

  SetRandomSeed(time(NULL));
  {
    Game game;

    while (!WindowShouldClose()) {
      float dt = GetFrameTime();

      game.update(dt);

      BeginDrawing();

      ClearBackground(BLACK);
      game.draw();
      EndDrawing();
    }
  }

  CloseWindow();
  return 0;
};
