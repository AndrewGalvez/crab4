#pragma once

#include "component.hpp"
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
enum Tile : unsigned char { TILE_EMPTY = 0, TILE_WALL = 1 };

class TileMap : public Component {
public:
  int width = 0;
  int height = 0;
  float tileSize;
  Vector2 origin = Vector2Zero();
  std::vector<unsigned char> tiles;

  explicit TileMap(float tileSize, Vector2 origin = {0, 0})
      : tileSize(tileSize), origin(origin) {}

  void loadFromStrings(const std::vector<std::string> &rows);
  void loadFromFile(std::string file);
  unsigned char at(int tx, int ty) const;
  bool isSolid(int tx, int ty) const { return at(tx, ty) == TILE_WALL; }
  bool isSolidAtPoint(Vector2 p) const;
  Rectangle tileRect(int tx, int ty) const;

  void collide(GameObject *solid);
  void draw() override;
};
