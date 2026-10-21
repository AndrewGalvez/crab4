#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>

struct SoundPool {
  std::vector<Sound> voices;
  size_t next = 0;
};

class AssetManager {
public:
  AssetManager() = default;
  ~AssetManager() { unloadAll(); };
  AssetManager(const AssetManager &) = delete;
  AssetManager &operator=(const AssetManager &) = delete;

  Texture2D *texture(const std::string &path);

  void unloadAll();

  Texture2D fallback = LoadTexture("assets/Missing_Texture.png");

  std::unordered_map<std::string, SoundPool> soundPools;
  void playSound(const std::string &path, int voices = 8);

private:
  std::unordered_map<std::string, Texture2D> textures;
};
