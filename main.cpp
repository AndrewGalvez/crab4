#include "game.hpp"
#include "raylib.h"

int main() {
  InitWindow(800, 600, "crab 4");
  SetTargetFPS(60);

  SetRandomSeed(time(NULL));

  ChangeDirectory(GetApplicationDirectory());

  InitAudioDevice();

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

  CloseAudioDevice();

  CloseWindow();
  return 0;
};
