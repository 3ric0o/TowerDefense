#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"
#include "WalkabilityMap.h"

struct SpawnPoint
{
    int tileX;
    int tileY;
    
    SpawnPoint(int x, int y) : tileX(x), tileY(y) {}
};

class EnemySpawner
{
public:
    EnemySpawner(WalkabilityMap& walkabilityMap, float tileSize = 32.0f, float scale = 2.0f);
    void AddSpawnPoint(int tileX, int tileY);
    void SetSpawnInterval(float minTime, float maxTime);
    void Update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies);
    
private:
    WalkabilityMap& walkabilityMap;
    std::vector<SpawnPoint> spawnPoints;
    float tileSize;
    float scale;
    
    float minSpawnTime;
    float maxSpawnTime;
    float spawnTimer;
    float timeToNextSpawn;
    
    SpawnPoint GetRandomSpawnPoint() const;
    Vector2 TileToWorld(int tileX, int tileY) const;
    std::unique_ptr<Enemy> CreateRandomEnemy(float x, float y) const;
};