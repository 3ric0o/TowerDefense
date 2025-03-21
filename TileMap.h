#pragma once
#include "raylib.h"
#include "WalkabilityMap.h"
#include "enemy.h"


struct Tile;

class TileMap
{
public:
  TileMap(const char* mapPath, const char* tilesetPath, int width, int height, int tileSize);
  ~TileMap();
  
  void Draw(int x_offset, int y_offset);
  void DrawObjects(int x_offset, int y_offset, Texture2D objectTexture, int objectWidth, int objectHeight);
  
  bool HighlightTileUnderMouse(int x_offset, int y_offset, Color highlightColor);
  
  Tile* GetTileAt(int x, int y) const;
  bool GetTileCoordinates(int screenX, int screenY, int x_offset, int y_offset, int* outTileX, int* outTileY);
  
  int GetWidth() const { return width; }
  int GetHeight() const { return height; }
  int GetTileSize() const { return tileSize; }
  float GetScale() const { return scale; }
  void UpdateWalkabilityMap(WalkabilityMap& walkMap, bool makeUnwalkable = true);
  
private:
  Tile* layers;
  Texture2D tileset;
  int width;
  int height;
  int tileSize;
  float scale;
};

// std::map<int,TileData> tileSetData;
// movementCosts[37] = 1.0f;

struct Tile {
  int id;
  float movementCost;
  bool isWalkable;
};