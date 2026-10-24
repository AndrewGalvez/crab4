#include "prefabs.hpp"
#include "camera.hpp"
#include "components.hpp"
#include "enemies.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "gold.hpp"
#include "player.hpp"
#include "renderers.hpp"
#include "scene.hpp"

namespace prefabs {

GameObject &enemy(Scene &scene, float x, float y, float w, float h,
                  c_Transform *target) {

  auto &e = scene.create();
  e.add<c_Transform>(Vector2{x, y}, Vector2{w, h});
  e.add<RectangleRenderer>(Rectangle{x, y, w, h}, GREEN);
  e.add<EnemyController>(200.0f, target);
  e.add<Health>(50);
  e.add<Velocity>(Vector2{0, 0});
  e.add<PhysicsManager>();
  e.add<Friction>(200.0f);
  return e;
};

GameObject &bullet(Scene &scene, Vector2 pos, float damage, float speed) {
  auto &b = scene.create();
  float sx = 20;
  float sy = 20;
  float x = pos.x;
  float y = pos.y;

  b.add<c_Transform>(Vector2{x, y}, Vector2{sx, sy});
  Vector2 v = Vector2Scale(
      Vector2Normalize(Vector2Subtract(
          GetScreenToWorld2D(GetMousePosition(),
                             scene.findAllWithComponent<c_Camera>()[0]
                                 ->get<c_Camera>()
                                 ->raw()),
          (Vector2){x, y})),
      speed);

  b.add<Velocity>(v);
  b.add<Lifetime>(100);
  b.add<PhysicsManager>();
  b.add<RectangleRenderer>((Rectangle){x, y, sx, sy}, BLUE);
  b.add<Bullet>(damage);

  return b;
};

GameObject &gold(Scene &scene, Vector2 center) {
  auto &e = scene.create();
  e.add<Gold>();
  e.add<c_Transform>(Vector2{center.x - 20, center.y - 20}, (Vector2){40, 40});
  e.add<Lifetime>(60000);
  e.add<Velocity>(Vector2{0, 0});
  e.add<ImageRenderer>(
      "assets/gold.png", Rectangle{center.x - 20, center.y - 20, 40, 40},
      Rectangle{0, 0, 16, 16}, 0, 10, Vector2{0, 0}, 0.0f,
      [](ImageRenderer &self) {
        self.source = Rectangle{16.0f * self.animFrame, 0, 16, 16};
      },
      0, 8);

  return e;
};

GameObject &deadEnemy(Scene &scene, Rectangle rect) {
  auto &d = scene.create();
  d.add<RectangleRenderer>(rect, RED);
  d.add<Lifetime>(60);
  d.add<DeadEnemy>();

  return d;
};

GameObject &player(Scene &scene, PlayerStats *stats) {
  auto &player = scene.create();
  player.add<c_Transform>((Vector2){0, 0}, (Vector2){100, 100});
  player.add<PlayerMagnetCircleDisplayRenderer>(Vector2{0, 0}, 5000.0f,
                                                ColorAlpha(BLUE, 100));
  player.add<RectangleRenderer>((Rectangle){0, 0, 100, 100}, RED);
  player.add<UIPositionLabel>();
  player.add<PlayerController>(stats);
  player.add<Velocity>(Vector2{0.0f, 0.0f});
  player.add<Friction>(400.0f);
  player.add<PhysicsManager>();
  return player;
};
} // namespace prefabs
