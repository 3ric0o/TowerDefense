#include "raylib.h"
#include "TileMap.h"
#include "Enemy.h"
#include "EnemySpawner.h"




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

    EnemySpawner spawner(32.0f, 2.0f);
    spawner.AddSpawnPoint(3, 1);
    spawner.AddSpawnPoint(4, 1);
    spawner.AddSpawnPoint(0, 9);
    spawner.AddSpawnPoint(0, 1);
    spawner.AddSpawnPoint(10, 19);
    spawner.AddSpawnPoint(11, 19);
    
    spawner.SetSpawnInterval(3.0f, 7.0f);
    
    // Vector to store active enemies
    std::vector<std::unique_ptr<Enemy>> enemies;
    
    while (!WindowShouldClose())  
    {
        float deltaTime = GetFrameTime();
        spawner.Update(deltaTime, enemies);

        // Update all enemies
        for (auto it = enemies.begin(); it != enemies.end();)
        {
            auto& enemy = *it;
            
            enemy->Update(deltaTime);
            
            if (!enemy->IsAlive() && enemy->IsDeathAnimationFinished())
            {
                it = enemies.erase(it);
            }
            else { ++it; }
        }
        BeginDrawing();
        ClearBackground(BLACK);

        ground.Draw(0, 0);
        fences.Draw(0, 0);
        treesMap.DrawObjects(0, 0, treeTexture, 66, 77);
        housesMap.DrawObjects(96, 0, houseTexture, 154, 149);
        ground.HighlightTileUnderMouse(0, 0, highlightColor);
        
        EndDrawing();
    }
    
    CloseWindow();     
    return 0;
}
