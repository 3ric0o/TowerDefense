#include "EnemySpawner.h"
#include "Blob.h"
#include "Goblin.h"
#include "Rat.h"
#include <cstdlib>
#include <ctime>

EnemySpawner::EnemySpawner(float tileSize, float scale)
    : tileSize(tileSize), scale(scale),
      minSpawnTime(2.0f), maxSpawnTime(5.0f),
      spawnTimer(0.0f)
{
    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Set first spawn time
    timeToNextSpawn = minSpawnTime + static_cast<float>(rand()) / (RAND_MAX / (maxSpawnTime - minSpawnTime));
}

void EnemySpawner::AddSpawnPoint(int tileX, int tileY)
{
    spawnPoints.emplace_back(tileX, tileY);
}

void EnemySpawner::SetSpawnInterval(float minTime, float maxTime)
{
    minSpawnTime = minTime;
    maxSpawnTime = maxTime;
}

void EnemySpawner::Update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies)
{
    if (spawnPoints.empty()) { return; }
    
    spawnTimer += deltaTime;
    
    if (spawnTimer >= timeToNextSpawn)
        {
        SpawnPoint point = GetRandomSpawnPoint();
        Vector2 worldPos = TileToWorld(point.tileX, point.tileY);
        
        // Create and add an enemy
        auto enemy = CreateRandomEnemy(worldPos.x, worldPos.y);
        enemies.push_back(std::move(enemy));
        
        spawnTimer = 0.0f;
        timeToNextSpawn = minSpawnTime + static_cast<float>(rand()) / (RAND_MAX / (maxSpawnTime - minSpawnTime));
    }
}

SpawnPoint EnemySpawner::GetRandomSpawnPoint() const
{
    int index = rand() % spawnPoints.size();
    return spawnPoints[index];
}

Vector2 EnemySpawner::TileToWorld(int tileX, int tileY) const
{
    Vector2 worldPos;
    worldPos.x = tileX * tileSize * scale + (tileSize * scale / 2.0f);
    worldPos.y = tileY * tileSize * scale + (tileSize * scale / 2.0f);
    return worldPos;
}

std::unique_ptr<Enemy> EnemySpawner::CreateRandomEnemy(float x, float y) const
{
    // Randomly choose an enemy type
    int enemyType = rand() % 3;

    std::unique_ptr<Enemy> enemy;
    
    switch(enemyType)
    {
    case 0:
        return std::make_unique<Blob>(x, y);
    case 1:
        return std::make_unique<Goblin>(x, y);
    case 2:
        return std::make_unique<Rat>(x, y);
    }
    return enemy;
}