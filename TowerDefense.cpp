#include "raylib.h"
#include <string>

#include "TileMap.h"


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
    
    
    while (!WindowShouldClose())  
    {
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
