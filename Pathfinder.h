#pragma once
#include "WalkabilityMap.h"
#include "raylib.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include "TileMap.h"

struct PathNode {
    int x, y;
    float gCost; // Cost from start
    float hCost; // Heuristic cost to end
    float movementCost = 1;
    
    // Get total cost (f = g + h)
    float GetFCost() const { return gCost + hCost; }
    bool operator==(const PathNode& other) const { return x == other.x && y == other.y; }
};

// Compare function for priority queue
struct ComparePathNode
{
    bool operator()(const PathNode& a, const PathNode& b)
    {
        // First compare by fCost
        if (std::abs(a.GetFCost() - b.GetFCost()) > 0.001f)
        {
            return a.GetFCost() > b.GetFCost();
        }
        // If fCost is the same, prioritize lower hCost
        return a.hCost > b.hCost;
    }
};

class Pathfinder
{
public:
    Pathfinder(const WalkabilityMap& walkMap, const TileMap& tileMap);
    
    // Find a path from start to end
    std::vector<Vector2> FindPath(int startX, int startY, int endX, int endY);
    void SetTargetLocation(int x, int y);
    Vector2 GetTargetLocation() const;
    
    // Debug 
    void DebugDrawPath(const std::vector<Vector2>& path, int tileSize, float scale, Color pathColor) const;

private:
    const WalkabilityMap& walkMap;
    const TileMap& tileMap;
    Vector2 targetLocation;
    float GetTileMovementCost(int x, int y) const;
    
    // Manhattan distance heuristic
    static float CalculateHCost(int x1, int y1, int x2, int y2);
    std::vector<PathNode> GetNeighbors(const PathNode& node) const;
    
    // Reconstruct the path from the came-from map
    std::vector<Vector2> ReconstructPath(
        std::unordered_map<int, std::pair<int, int>>& cameFrom, 
        int startX, int startY, int endX, int endY) const;
};