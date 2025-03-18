#include "raylib.h"
#include "TileMap.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "WalkabilityMap.h"
#include "Pathfinder.h"
#include "Player.h"

int main()
{
    // Initialization
    constexpr int screen_width { 1888 };
    constexpr int screen_height { 1280 };

    InitWindow(screen_width, screen_height, "Tower Defense Game");
    SetTargetFPS(60);

    Player player(100);
    std::vector<std::unique_ptr<Enemy>> enemies;

    auto ResetGame = [&]()
    {
        player = Player(100);
        enemies.clear();
    };
    
    TileMap ground("resources/Layers/1.csv", "resources/fieldstileset.png", 30, 20, 32);
    TileMap fences("resources/Layers/2.csv", "resources/VILLAGE TILESET/1.1 Tiles/Tileset2.png", 30, 20, 32);

    // Empty tileset path since we'll use DrawObjects
    TileMap treesMap("resources/Layers/3.csv", "", 30, 20, 32);  
    TileMap housesMap("resources/Layers/4.csv", "", 30, 20, 32);
    TileMap wellMap("resources/Layers/5.csv", "", 30, 20, 32);
    TileMap boxesMap("resources/Layers/6.csv", "", 30, 20, 32);
    TileMap tree2Map("resources/Layers/7.csv", "", 30, 20, 32);
    TileMap bush1Map("resources/Layers/8.csv", "", 30, 20, 32);
    TileMap bush2Map("resources/Layers/9.csv", "", 30, 20, 32);
    TileMap flower1Map("resources/Layers/10.csv", "", 30, 20, 32);
    TileMap flower2Map("resources/Layers/11.csv", "", 30, 20, 32);
    
    Texture2D treeTexture = LoadTexture("resources/FIELDS TILESET/2 Objects/7 Decor/Tree1.png");
    Texture2D houseTexture = LoadTexture("resources/VILLAGE TILESET/2 Objects/7 House/4.png");
    Texture2D wellTexture = LoadTexture("resources/VILLAGE TILESET/2 Objects/3 Decor/13.png");
    Texture2D boxTexture = LoadTexture("resources/FIELDS TILESET/2 Objects/7 Decor/Box2.png");
    Texture2D tree2Texture = LoadTexture("resources/FIELDS TILESET/2 Objects/7 Decor/Tree2.png");
    Texture2D bush1Texture = LoadTexture("resources/FIELDS TILESET/2 Objects/9 Bush/2.png");
    Texture2D bush2Texture = LoadTexture("resources/FIELDS TILESET/2 Objects/9 Bush/1.png");
    Texture2D flower1Texture = LoadTexture("resources/FIELDS TILESET/2 Objects/6 Flower/9.png");
    Texture2D flower2Texture = LoadTexture("resources/FIELDS TILESET/2 Objects/6 Flower/1.png");
    
    Color highlightColor = Color(RED);

    WalkabilityMap walkMap(ground.GetWidth(), ground.GetHeight());
    
    walkMap.Reset();  // Start with all walkable
    fences.UpdateWalkabilityMap(walkMap); 
    treesMap.UpdateWalkabilityMap(walkMap); 
    housesMap.UpdateWalkabilityMap(walkMap);  
    wellMap.UpdateWalkabilityMap(walkMap);  
    boxesMap.UpdateWalkabilityMap(walkMap);  
    tree2Map.UpdateWalkabilityMap(walkMap);
    bush1Map.UpdateWalkabilityMap(walkMap);
    bush2Map.UpdateWalkabilityMap(walkMap);

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
    pathfinder.SetTargetLocation(26, 10);
    
    bool gameActive = true;
    
    while (!WindowShouldClose())  
    {
        float deltaTime = GetFrameTime();

        if (!player.IsAlive() && IsKeyPressed(KEY_R))
        {
            ResetGame();
            gameActive = true;
        }
        

        if (gameActive && player.IsAlive())
        {
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
            }

            // Update all enemies
            for (auto it = enemies.begin(); it != enemies.end();)
            {
                auto& enemy = *it;
    
                // Get enemy position in tile coordinates
                int tileX = static_cast<int>(enemy->GetPosition().x / (32 * 2.0f));
                int tileY = static_cast<int>(enemy->GetPosition().y / (32 * 2.0f));
    
                Tile* currentTile = ground.GetTileAt(tileX, tileY);
            
                Vector2 targetPos = pathfinder.GetTargetLocation();
                if (tileX == targetPos.x && tileY == targetPos.y)
                {
                    // Enemy reached the end point, remove it
                    player.TakeDamage(enemy->GetDamage());
                    it = enemies.erase(it);

                    if (!player.IsAlive())
                    {
                        gameActive = false;
                    }
                    continue;
                }
    
                if (currentTile && currentTile->id == 37)
                {
                    enemy->SetSpeedMultiplier(0.6f);
                }
                else
                {
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
        }
        BeginDrawing();
        ClearBackground(BLACK);

        ground.Draw(0, 0);
        fences.Draw(0, 0);
        treesMap.DrawObjects(0, 0, treeTexture, 66, 77);
        housesMap.DrawObjects(96, 0, houseTexture, 154, 149);
        wellMap.DrawObjects(0, 0, wellTexture, 43, 54);
        boxesMap.DrawObjects(0, 0, boxTexture, 18, 18);
        tree2Map.DrawObjects(0, 0, tree2Texture, 29, 26);
        bush1Map.DrawObjects(0, 0, bush1Texture, 37, 26);
        bush2Map.DrawObjects(0, 0, bush2Texture, 26, 23);
        flower1Map.DrawObjects(0, 0, flower1Texture, 8, 7);
        flower2Map.DrawObjects(0, 0, flower2Texture, 6, 6);
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
        
        player.Draw();
        EndDrawing();
    }
    
    CloseWindow();     
    return 0;
}
