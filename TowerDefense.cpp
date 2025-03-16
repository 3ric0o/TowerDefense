#include "raylib.h"
#include "TileMap.h"
#include "Enemy.h"
#include "Blob.h"



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
    
    Texture2D treeTexture = LoadTexture("resources/FIELDS TILESET/2 Objects/7 Decor/Tree1.png"); // 66x77
    Texture2D houseTexture = LoadTexture("resources/VILLAGE TILESET/2 Objects/7 House/4.png"); // 154x149
    
    Color highlightColor = Color(RED);


    // Create a blob enemy for testing
    // Convert tile coordinates to pixel coordinates (3,1)
    float spawnX = 3 * 32 * 2.0f + 32; // Adding 32 to center in the tile
    float spawnY = 1 * 32 * 2.0f + 32;
    
    Blob testBlob(spawnX, spawnY);
    
    // Set target position to walk downward to row 15
    float targetX = spawnX; // Same X coordinate (straight down)
    float targetY = 15 * 32 * 2.0f + 32;
    testBlob.SetTargetPosition(targetX, targetY);
    
    
    while (!WindowShouldClose())  
    {
        float deltaTime = GetFrameTime();
        testBlob.Update(deltaTime);
        
        BeginDrawing();
        ClearBackground(BLACK);

        ground.Draw(0, 0);
        fences.Draw(0, 0);
        treesMap.DrawObjects(0, 0, treeTexture, 66, 77);
        housesMap.DrawObjects(96, 0, houseTexture, 154, 149);
        ground.HighlightTileUnderMouse(0, 0, highlightColor);

        testBlob.Draw();
        
        EndDrawing();
    }
    
    CloseWindow();     
    return 0;
}
