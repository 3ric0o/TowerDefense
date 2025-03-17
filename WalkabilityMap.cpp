#include "WalkabilityMap.h"

WalkabilityMap::WalkabilityMap(int width, int height)
    : width(width), height(height)
{
    walkable.resize(height, std::vector(width, true));
}

void WalkabilityMap::Reset()
{
    // Reset all tiles to walkable
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            walkable[y][x] = true;
        }
    }
}

void WalkabilityMap::SetUnwalkable(int x, int y)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        walkable[y][x] = false;
    }
}

bool WalkabilityMap::IsWalkable(int x, int y) const
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return walkable[y][x];
    }
    return false; // Tiles outside the map are not walkable
}
void WalkabilityMap::ForceWalkable(int x, int y)
{
    walkable[y][x] = true;
}