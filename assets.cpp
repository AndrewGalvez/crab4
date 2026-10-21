#include "assets.hpp"
#include <raylib.h>

Texture2D *AssetManager::texture(const std::string &path) {
  auto it = textures.find(path);
  if (it != textures.end())
    return &it->second;

  Texture2D t = LoadTexture(path.c_str());
  if (t.id == 0) {
    TraceLog(LOG_WARNING, "MISSING TEXTURE: %s", path.c_str());
    return &fallback;
  }
  return &textures.emplace(path, t).first->second;
}

void AssetManager::unloadAll() {
  for (auto &[k, v] : textures)
    UnloadTexture(v);
  UnloadTexture(fallback);
  textures.clear();
  fallback = {};
};

void AssetManager::playSound(const std::string &path, int voices) {
  auto it = soundPools.find(path);
  if (it == soundPools.end()) {
    SoundPool p;
    p.voices.push_back(LoadSound(path.c_str()));
    for (int i = 1; i < voices; i++)
      p.voices.push_back(LoadSoundAlias(p.voices[0]));
    it = soundPools.emplace(path, std::move(p)).first;
  }
  auto &p = it->second;
  PlaySound(p.voices[p.next]);
  p.next = (p.next + 1) % p.voices.size();
}
