#include "raylib.h"
#include "TileMap.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "WalkabilityMap.h"
#include "Pathfinder.h"




int main()
{
    // Initialization
    constexpr int screen_width { 1888 };
    constexpr int screen_height { 1280 };

    InitWindow(screen_width, screen_height, "Tower Defense Game");
    SetTargetFPS(60);

    TileMap ground("resources/Layers/1.csv", "resources/fieldstileset.png", 30, 20, 32);
    TileMap fences("resources/Layers/2.csv", "resources/VILLAGE TILESET/1.1 Tiles/Tileset2.png", 30, 20, 32);

    // Empty tileset path since we'll use DrawObjects
    TileMap treesMap("resources/Layers/3.csv", "", 30, 20, 32);  
    TileMap housesMap("resources/Layers/4.csv", "", 30, 20, 32); 
    
    Texture2D treeTexture = LoadTexture("resources/FIELDS TILESET/2 Objects/7 Decor/Tree1.png");
    Texture2D houseTexture = LoadTexture("resources/VILLAGE TILESET/2 Objects/7 House/4.png"); 
    
    Color highlightColor = Color(RED);

    WalkabilityMap walkMap(ground.GetWidth(), ground.GetHeight());
    
    walkMap.Reset();  // Start with all walkable
    fences.UpdateWalkabilityMap(walkMap);  // Make fence tiles unwalkable
    treesMap.UpdateWalkabilityMap(walkMap);  // Make tree tiles unwalkable
    housesMap.UpdateWalkabilityMap(walkMap);  // Make house tiles unwalkable

    walkMap.ForceWalkable(27, 10);
    walkMap.ForceWalkable(27, 11);
    walkMap.ForceWalkable(26, 10);
    walkMap.ForceWalkable(26, 11);
    walkMap.ForceWalkable(10, 18);
    walkMap.ForceWalkable(11, 18);

    
    EnemySpawner spawner(walkMap,32.0f, 2.0f);
    spawner.AddSpawnPoint(3, 1);
    spawner.AddSpawnPoint(4, 1);
    spawner.AddSpawnPoint(0, 9);
    spawner.AddSpawnPoint(0, 10);
    spawner.AddSpawnPoint(10, 19);
    spawner.AddSpawnPoint(11, 19);
    
    spawner.SetSpawnInterval(3.0f, 7.0f);

    Pathfinder pathfinder(walkMap, ground);
    pathfinder.SetTargetLocation(26, 11);
    
    // Vector to store active enemies
    std::vector<std::unique_ptr<Enemy>> enemies;
    
    while (!WindowShouldClose())  
    {
        float deltaTime = GetFrameTime();
        spawner.Update(deltaTime, enemies);

        for (auto& enemy : enemies)
        {
            if (!enemy->HasPath() && enemy->IsAlive())
            {
                // Get the enemy's position in grid coordinates
                int tileX = static_cast<int>(enemy->GetPosition().x / (32 * 2.0f));
                int tileY = static_cast<int>(enemy->GetPosition().y / (32 * 2.0f));
                
                Vector2 targetPos = pathfinder.GetTargetLocation();
                std::vector<Vector2> path = pathfinder.FindPath(tileX, tileY, targetPos.x, targetPos.y);
                enemy->SetPath(path);
            }
            // if enemy position == tile position with id == 37 enemy speed reduced by 10%
        }

        // Update all enemies
        for (auto it = enemies.begin(); it != enemies.end();)
        {
            auto& enemy = *it;
    
            // Get enemy position in tile coordinates
            int tileX = static_cast<int>(enemy->GetPosition().x / (32 * 2.0f));
            int tileY = static_cast<int>(enemy->GetPosition().y / (32 * 2.0f));
    
            // Check if enemy is on a slow tile (ID 37)
            Tile* currentTile = ground.GetTileAt(tileX, tileY);
    
            if (currentTile && currentTile->id == 37)
            {
                enemy->SetSpeedMultiplier(0.6f);
            }
            else
            {
                // Reset to normal speed
                enemy->ResetSpeed();
            }
    
            enemy->Update(deltaTime);
    
            if (!enemy->IsAlive() && enemy->IsDeathAnimationFinished())
            {
                it = enemies.erase(it);
            }
            else
            {
                ++it;
            }
        }
        BeginDrawing();
        ClearBackground(BLACK);

        ground.Draw(0, 0);
        fences.Draw(0, 0);
        treesMap.DrawObjects(0, 0, treeTexture, 66, 77);
        housesMap.DrawObjects(96, 0, houseTexture, 154, 149);
        //ground.HighlightTileUnderMouse(0, 0, highlightColor);
        //walkMap.DrawDebugOverlay(0, 0, ground.GetTileSize(), ground.GetScale());

        for (auto& enemy : enemies) { enemy->Draw(); }
        /*Vector2 target = pathfinder.GetTargetLocation();
        DrawRectangle(
            target.x * ground.GetTileSize() * ground.GetScale(),
            target.y * ground.GetTileSize() * ground.GetScale(),
            ground.GetTileSize() * ground.GetScale(),
            ground.GetTileSize() * ground.GetScale(),
            ColorAlpha(YELLOW, 0.5f)
        );*/
        
        EndDrawing();
    }
    
    CloseWindow();     
    return 0;
}
