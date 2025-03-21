#include "Tower.h"

Projectile::Projectile(Vector2 start, Vector2 target, float speed, int damage)
    : position(start), speed(speed), damage(damage), active(true), radius(5.0f)
{
    direction = {target.x - start.x, target.y - start.y};
    
    float magnitude = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (magnitude > 0) {
        direction.x /= magnitude;
        direction.y /= magnitude;
    }
}

void Projectile::Update(float deltaTime)
{
    if (!active) return;
    
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;
}

void Projectile::Draw() const
{
    if (!active) return;
    
    DrawCircleV(position, radius, RED);
}

bool Projectile::IsActive() const
{
    return active;
}

Vector2 Projectile::GetPosition() const
{
    return position;
}

int Projectile::GetDamage() const
{
    return damage;
}

static Texture2D towerTexture = { 0 };
static int towerInstanceCount = 0;

Tower::Tower(float x, float y, float radius)
    : position({x, y}), radius(radius), active(true),
      frameCount(4), currentFrame(0), framesCounter(0), framesSpeed(8),
      attackCooldown(1.0f), attackTimer(0.0f), attackRate(1.0f)
{
    if (towerTexture.id == 0)
    {
        towerTexture = LoadTexture("resources/ARCHER TOWER/2 Idle/2.png");
    }
    texture = towerTexture;
    towerInstanceCount++;

    frameWidth = texture.width / frameCount;
}

Tower::~Tower()
{
    towerInstanceCount--;
    if (towerInstanceCount == 0 && texture.id != 0) {
        UnloadTexture(towerTexture);
        towerTexture = {0};
    }
}

void Tower::Update(float deltaTime)
{
    framesCounter++;

    if (framesCounter >= (60 / framesSpeed))
    {
        framesCounter = 0;
        currentFrame++;

        if (currentFrame >= frameCount)
            currentFrame = 0;
    }
    
    attackTimer += deltaTime;
    
    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        it->Update(deltaTime);
        
        Vector2 projectilePos = it->GetPosition();
        float dx = projectilePos.x - position.x;
        float dy = projectilePos.y - position.y;
        float distanceSq = dx * dx + dy * dy;
        
        if (distanceSq > radius * radius * 1.5f)
        {
            it = projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Tower::Draw() const
{
    if (!active) return;

    // Draw tower with animation
    Rectangle source = {
        static_cast<float>(currentFrame * frameWidth),
        0,
        static_cast<float>(frameWidth),
        static_cast<float>(texture.height)
    };

    Rectangle dest = {
        position.x,
        position.y - 64.0f,
        frameWidth * scale,
        texture.height * scale
    };

    Vector2 origin = {
        (frameWidth * scale) / 2.0f,
        (texture.height * scale) / 2.0f
    };

    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);

    DrawCircleV(position, radius, ColorAlpha(SKYBLUE, 0.2f));
    DrawCircleLines(position.x, position.y, radius, SKYBLUE);
    
    for (const auto& projectile : projectiles)
    {
        projectile.Draw();
    }
}

void Tower::CheckEnemiesInRange(const std::vector<std::unique_ptr<Enemy>>& enemies)
{
    if (!active) return;
    
    if (attackTimer < attackCooldown) return;
    
    Enemy* closestEnemy = nullptr;
    float closestDistanceSq = radius * radius;
    
    for (const auto& enemy : enemies)
    {
        if (!enemy->IsAlive()) continue;
        
        Vector2 enemyPos = enemy->GetPosition();
        float dx = enemyPos.x - position.x;
        float dy = enemyPos.y - position.y;
        float distanceSq = dx * dx + dy * dy;
        
        if (distanceSq <= radius * radius && distanceSq < closestDistanceSq)
        {
            closestDistanceSq = distanceSq;
            closestEnemy = enemy.get();
        }
    }
    
    if (closestEnemy)
    {
        Vector2 targetPos = closestEnemy->GetPosition();
        
        projectiles.emplace_back(position, targetPos);
        
        attackTimer = 0.0f;
    }
}

bool Tower::CanPlaceAt(int tileX, int tileY, const WalkabilityMap& walkMap)
{
    for (int y = 0; y < TOWER_HEIGHT; y++)
    {
        for (int x = 0; x < TOWER_WIDTH; x++)
        {
            if (!walkMap.IsWalkable(tileX + x, tileY + y))
            {
                return false;
            }
        }
    }
    return true;
}

void Tower::DrawPlacementPreview(int tileX, int tileY, const WalkabilityMap& walkMap)
{
    if (towerTexture.id == 0)
    {
        towerTexture = LoadTexture("resources/ARCHER TOWER/2 Idle/2.png");
    }
    
    const int tileSize = 32;
    const float scale = 2.0f;

    // Calculate world position
    float worldX = tileX * tileSize * scale + (tileSize * scale * TOWER_WIDTH / 2.0f);
    float worldY = tileY * tileSize * scale + (tileSize * scale * TOWER_HEIGHT / 2.0f);

    bool canPlace = CanPlaceAt(tileX, tileY, walkMap);
    Color overlayColor = canPlace ? ColorAlpha(GREEN, 0.5f) : ColorAlpha(RED, 0.5f);
    
    for (int y = 0; y < TOWER_HEIGHT; y++)
    {
        for (int x = 0; x < TOWER_WIDTH; x++)
        {
            DrawRectangle(
                (tileX + x) * tileSize * scale,
                (tileY + y) * tileSize * scale,
                tileSize * scale,
                tileSize * scale,
                overlayColor
            );
        }
    }
    
    DrawCircleV({worldX, worldY}, 250.0f, ColorAlpha(SKYBLUE, 0.2f));
    DrawCircleLines(worldX, worldY, 250.0f, SKYBLUE);
    
    if (canPlace && towerTexture.id != 0) {
        Rectangle source = {0, 0, static_cast<float>(towerTexture.width / 4), static_cast<float>(towerTexture.height)};
        Rectangle dest = {worldX, worldY - 64.0f, (towerTexture.width / 4) * scale, towerTexture.height * scale};
        Vector2 origin = {(towerTexture.width / 4 * scale) / 2.0f, (towerTexture.height * scale) / 2.0f};

        DrawTexturePro(towerTexture, source, dest, origin, 0.0f, ColorAlpha(WHITE, 0.7f));
    }
}

Vector2 Tower::GetPosition() const
{
    return position;
}

float Tower::GetRadius() const
{
    return radius;
}

bool Tower::IsActive() const
{
    return active;
}

const std::vector<Projectile>& Tower::GetProjectiles() const
{
    return projectiles;
}