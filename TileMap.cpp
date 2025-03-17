#include "TileMap.h"
#include <string>

TileMap::TileMap(const char* mapPath, const char* tilesetPath, int width, int height, int tileSize) :
    width(width), height(height), tileSize(tileSize), scale(2.0f)
{
    tileset = LoadTexture(tilesetPath);
    layers = static_cast<Tile*>(calloc(width * height, sizeof(Tile)));
    
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, mapPath, "r");
    
    if (err != 0 || file == nullptr) { return; }
    
    int num_read { 0 };
    while (fscanf_s(file, "%d,", &layers[num_read].id) != EOF)
    {
        num_read++;
    }
    fclose(file);
}

TileMap::~TileMap()
{
    UnloadTexture(tileset);
    free(layers);
}

void TileMap::Draw(int x_offset, int y_offset)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Tile* tile = &layers[y * width + x];
            
            // Skip drawing for tiles with ID -1
            if (tile->id == -1) continue;
            if (tile->id == 37) { tile->movementCost = 1.9f; }
            
            // Apply scaling to screen coordinates
            int screenX = x * tileSize * scale + x_offset;
            int screenY = y * tileSize * scale + y_offset;

            int tilemap_y = tile->id / (tileset.width / tileSize);
            int tilemap_x = tile->id % (tileset.width / tileSize);

            const Rectangle source =
            {
                static_cast<float>(tilemap_x * tileSize),
                static_cast<float>(tilemap_y * tileSize),
                static_cast<float>(tileSize),
                static_cast<float>(tileSize)
            };
            
            // Destination rectangle uses scaled dimensions
            const Rectangle dest = {
                static_cast<float>(screenX), 
                static_cast<float>(screenY), 
                (tileSize * scale), 
                (tileSize * scale)
            };
            
            // Draw the scaled tile
            DrawTexturePro(tileset, source, dest, Vector2{0, 0}, 0.0f, WHITE);
        }
    }
}

void TileMap::DrawObjects(int x_offset, int y_offset, Texture2D objectTexture, int objectWidth, int objectHeight)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Tile* tile = &layers[y * width + x];
            
            if (tile->id == 0)
            {
                int screenX = x * tileSize * scale + x_offset;
                int screenY = y * tileSize * scale + y_offset;
                
                int objOffsetX = (tileSize * scale - objectWidth * scale) / 2;
                int objOffsetY = (tileSize * scale - objectHeight * scale);
                
                Rectangle source = {0, 0, static_cast<float>(objectWidth), static_cast<float>(objectHeight)};
                
                Rectangle dest = {
                    static_cast<float>(screenX + objOffsetX), 
                    static_cast<float>(screenY + objOffsetY),
                    objectWidth * scale, 
                    objectHeight * scale
                };
                
                DrawTexturePro(objectTexture, source, dest, Vector2{0, 0}, 0.0f, WHITE);
            }
        }
    }
}

bool TileMap::HighlightTileUnderMouse(int x_offset, int y_offset, Color highlightColor)
{
    Vector2 mousePos = GetMousePosition();
    
    // mouse position to grid coordinates
    int tileX = (mousePos.x - x_offset) / (tileSize * scale);
    int tileY = (mousePos.y - y_offset) / (tileSize * scale);
    
    // Check if within bounds
    if (tileX >= 0 && tileX < width && tileY >= 0 && tileY < height) 
    {
        // Get the screen position of the tile
        int screenX = tileX * tileSize * scale + x_offset;
        int screenY = tileY * tileSize * scale + y_offset;
        
        DrawRectangle(
            screenX, 
            screenY, 
            tileSize * scale, 
            tileSize * scale, 
            highlightColor
        );
        //Debug - DELETE LATER!
        Tile* tile = &layers[tileY * width + tileX];
        std::string tileInfo = "Tile: " + std::to_string(tileX) + "," + std::to_string(tileY) + " ID: " + std::to_string(tile->id);
        
        DrawText(tileInfo.c_str(), 10, 10, 20, WHITE);
        
        return true;
    }
    
    return false;
}

Tile* TileMap::GetTileAt(int x, int y) const
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return &layers[y * width + x];
    }
    return nullptr;
}

bool TileMap::GetTileCoordinates(int screenX, int screenY, int x_offset, int y_offset, int* outTileX, int* outTileY)
{
    int tileX = (screenX - x_offset) / (tileSize * scale);
    int tileY = (screenY - y_offset) / (tileSize * scale);
    
    if (tileX >= 0 && tileX < width && tileY >= 0 && tileY < height) {
        *outTileX = tileX;
        *outTileY = tileY;
        return true;
    }
    
    return false;
}

void TileMap::UpdateWalkabilityMap(WalkabilityMap& walkMap, bool makeUnwalkable)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Tile* tile = GetTileAt(x, y);
            
            if (tile && tile->id != -1)
            {
                if (makeUnwalkable)
                {
                    walkMap.SetUnwalkable(x, y);
                }
            }
        }
    }
}