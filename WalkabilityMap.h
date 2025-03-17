#pragma once
#include <vector>
#include <raylib.h>


class WalkabilityMap {
public:
    WalkabilityMap(int width, int height);
    
    void Reset();
    void SetUnwalkable(int x, int y);
    bool IsWalkable(int x, int y) const;
    void ForceWalkable(int x, int y);
    
    // Getters
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    void DrawDebugOverlay(float offsetX, float offsetY, float tileSize, float scale) const;
private:
    std::vector<std::vector<bool>> walkable;
    int width;
    int height;
};