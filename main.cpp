#include "game.hpp"
#include "raylib.h"

int main() {
  InitWindow(800, 600, "crab 4");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetWindowMinSize(320, 240);
  SetTargetFPS(60);

  SetRandomSeed(time(NULL));

  ChangeDirectory(GetApplicationDirectory());

  InitAudioDevice();

  RenderTexture2D target = LoadRenderTexture(800, 600);
  SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

  {
    Game game;

    while (!WindowShouldClose()) {
      float dt = GetFrameTime();

      game.update(dt);

      BeginTextureMode(target);

      ClearBackground(BLACK);
      game.draw();

      EndTextureMode();

      float scale = std::min((float)GetScreenWidth() / 800,
                             (float)GetScreenHeight() / 600);
      float destW = 800 * scale;
      float destH = 600 * scale;
      float destX = (GetScreenWidth() - destW) * 0.5f;
      float destY = (GetScreenHeight() - destH) * 0.5f;

      SetMouseOffset((int)-destX, (int)-destY);
      SetMouseScale(1.0f / scale, 1.0f / scale);

      BeginDrawing();
      ClearBackground(BLACK);
      DrawTexturePro(
          target.texture,
          {0, 0, (float)target.texture.width, -(float)target.texture.height},
          {destX, destY, destW, destH}, {0, 0}, 0.0f, WHITE);
      EndDrawing();
    }
    UnloadRenderTexture(target);
  }

  CloseAudioDevice();

  CloseWindow();
  return 0;
};
