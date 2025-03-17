#include "Pathfinder.h"
#include <cmath>
#include <algorithm>
#include <queue>
#include <unordered_map>

#include "TileMap.h"

Pathfinder::Pathfinder(const WalkabilityMap& walkMap, const TileMap& tile_map) : walkMap(walkMap), tileMap(tile_map),
    targetLocation({26, 11})
{}

void Pathfinder::SetTargetLocation(int x, int y)
{
    targetLocation = Vector2(static_cast<float>(x), static_cast<float>(y));
}

Vector2 Pathfinder::GetTargetLocation() const { return targetLocation; }

float Pathfinder::CalculateHCost(int x1, int y1, int x2, int y2)
{
    return static_cast<float>(abs(x1 - x2) + abs(y1 - y2));
}

std::vector<PathNode> Pathfinder::GetNeighbors(const PathNode& node) const
{
    std::vector<PathNode> neighbors;
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};

    for (int i = 0; i < 4; ++i) {
        int newX = node.x + dx[i];
        int newY = node.y + dy[i];
        if (walkMap.IsWalkable(newX, newY))
        {
            float movementCost = GetTileMovementCost(newX, newY);
            neighbors.push_back({newX, newY, 0.0f, 0.0f, movementCost});
        }
    }
    return neighbors;
}

std::vector<Vector2> Pathfinder::FindPath(int startX, int startY, int endX, int endY)
{
    if (!walkMap.IsWalkable(startX, startY) || !walkMap.IsWalkable(endX, endY)) { return {}; }

    std::priority_queue<PathNode, std::vector<PathNode>, ComparePathNode> openSet;
    std::unordered_map<int, PathNode> openMap;
    std::unordered_map<int, bool> closedSet;
    std::unordered_map<int, std::pair<int, int>> cameFrom;
    int width = walkMap.GetWidth();

    PathNode startNode = {
        .x = startX,
        .y = startY,
        .gCost = 0.0f,
        .hCost = CalculateHCost(startX, startY, endX, endY),
        .movementCost = 1.0f
    };
    openSet.push(startNode);
    openMap[startX + startY * width] = startNode;

    while (!openSet.empty())
    {
        PathNode current = openSet.top();
        openSet.pop();
        openMap.erase(current.x + current.y * width);
        closedSet[current.x + current.y * width] = true;

        if (current.x == endX && current.y == endY)
        {
            return ReconstructPath(cameFrom, startX, startY, endX, endY);
        }

        for (const auto& neighbor : GetNeighbors(current))
        {
            int neighborKey = neighbor.x + neighbor.y * width;
            if (closedSet.find(neighborKey) != closedSet.end()) continue;
            
            float tentativeGCost = current.gCost + (1.0f * neighbor.movementCost);
            if (openMap.find(neighborKey) == openMap.end() || tentativeGCost < openMap[neighborKey].gCost)
            {
                PathNode updatedNeighbor = neighbor;
                updatedNeighbor.gCost = tentativeGCost;
                updatedNeighbor.hCost = CalculateHCost(neighbor.x, neighbor.y, endX, endY);
                updatedNeighbor.movementCost = neighbor.movementCost;
                openMap[neighborKey] = updatedNeighbor;
                openSet.push(updatedNeighbor);
                cameFrom[neighborKey] = {current.x, current.y};
            }
        }
    }
    return {};
}

std::vector<Vector2> Pathfinder::ReconstructPath(
    std::unordered_map<int, std::pair<int, int>>& cameFrom, 
    int startX, int startY, int endX, int endY) const
{
    std::vector<Vector2> path;
    int currentX = endX, currentY = endY, width = walkMap.GetWidth();

    while (currentX != startX || currentY != startY)
    {
        path.push_back({static_cast<float>(currentX), static_cast<float>(currentY)});
        int key = currentX + currentY * width;
        if (cameFrom.find(key) == cameFrom.end()) return {};
        auto prev = cameFrom[key];
        currentX = prev.first;
        currentY = prev.second;
    }
    path.push_back({static_cast<float>(startX), static_cast<float>(startY)});
    std::ranges::reverse(path);
    return path;
}

float Pathfinder::GetTileMovementCost(int x, int y) const
{
    Tile* tile = tileMap.GetTileAt(x, y);
    if (tile->id == 37)
    {  
        return 1.0f * tile->movementCost;
    }
    return 1.0f;  
}
