#include "tilemap.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "utils.hpp"
#include <cmath>
#include <raylib.h>
#include <sstream>
#include <string>

void TileMap::loadFromStrings(const std::vector<std::string> &rows) {
  height = (int)rows.size();
  width = 0;
  for (auto &r : rows)
    width = std::max(width, (int)r.size());

  tiles.assign(width * height, TILE_EMPTY);
  for (int y = 0; y < height; y++)
    for (int x = 0; x < (int)rows[y].size(); x++)
      if (rows[y][x] == '#')
        tiles[y * width + x] = TILE_WALL;
}

void TileMap::loadFromFile(std::string file) {
  std::string t = LoadFileText(file.c_str());
  std::vector<std::string> lines;
  std::stringstream ss(t);
  std::string line;

  while (std::getline(ss, line)) {
    lines.push_back(line);
  }

  loadFromStrings(lines);
};

unsigned char TileMap::at(int tx, int ty) const {
  if (tx < 0 || ty < 0 || tx >= width || ty >= height)
    return TILE_EMPTY;
  return tiles[ty * width + tx];
}

bool TileMap::isSolidAtPoint(Vector2 p) const {
  return isSolid((int)floorf((p.x - origin.x) / tileSize),
                 (int)floorf((p.y - origin.y) / tileSize));
  ;
}

Rectangle TileMap::tileRect(int tx, int ty) const {
  return {origin.x + tx * tileSize, origin.y + ty * tileSize, tileSize,
          tileSize};
}

void TileMap::collide(GameObject *s) {
  auto *st = s->get<c_Transform>();
  if (!st)
    return;
  auto *v = s->get<Velocity>();

  auto resolve = [&](bool xAxis) {
    Rectangle r = rectOf(s);
    int x0 = (int)floorf((r.x - origin.x) / tileSize);
    int y0 = (int)floorf((r.y - origin.y) / tileSize);
    int x1 = (int)floorf((r.x + r.width - origin.x) / tileSize);
    int y1 = (int)floorf((r.y + r.height - origin.y) / tileSize);

    for (int ty = y0; ty <= y1; ty++) {
      for (int tx = x0; tx <= x1; tx++) {
        if (!isSolid(tx, ty))
          continue;

        Rectangle m = rectOf(s); // recompute: earlier pushes moved us
        Rectangle t = tileRect(tx, ty);
        if (!CheckCollisionRecs(m, t))
          continue;

        // ignore float-epsilon "touching" overlaps
        Rectangle o = GetCollisionRec(m, t);
        if (o.width < 0.001f || o.height < 0.001f)
          continue;

        if (xAxis) {
          // snap exactly to the tile edge on the side we came from
          st->position.x = (m.x + m.width / 2 < t.x + t.width / 2)
                               ? t.x - m.width
                               : t.x + t.width;
          if (v)
            v->value.x = 0;
        } else {
          st->position.y = (m.y + m.height / 2 < t.y + t.height / 2)
                               ? t.y - m.height
                               : t.y + t.height;
          if (v)
            v->value.y = 0;
        }
      }
    }
  };

  // X pass: test the new X against last frame's Y
  float targetY = st->position.y;
  st->position.y = st->_pos.y;
  resolve(true);

  // Y pass: now apply the real Y with the corrected X
  st->position.y = targetY;
  resolve(false);

  st->_pos = st->position;
}
void TileMap::draw() {
  auto *cam = owner->scene->findFirstWithComponent<c_Camera>();
  if (!cam)
    return;
  const Camera2D &camraw = cam->get<c_Camera>()->raw();
  Vector2 tl = GetScreenToWorld2D((Vector2){0, 0}, camraw);
  Vector2 br = GetScreenToWorld2D(Vector2{800, 600}, camraw);
  int x0 = std::max(0, (int)floorf((tl.x - origin.x) / tileSize));
  int y0 = std::max(0, (int)floorf((tl.y - origin.y) / tileSize));
  int x1 = std::min(width - 1, (int)floorf((br.x - origin.x) / tileSize));
  int y1 = std::min(height - 1, (int)floorf((br.y - origin.y) / tileSize));

  for (int ty = y0; ty <= y1; ty++)
    for (int tx = x0; tx <= x1; tx++)
      if (isSolid(tx, ty))
        DrawRectangleRec(tileRect(tx, ty), DARKGRAY);
}
