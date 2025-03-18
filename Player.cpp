#include "Player.h"
#include <algorithm>
#include <string>

Player::Player(int initialHealth)
    : health(initialHealth), maxHealth(initialHealth)
{}

void Player::TakeDamage(int damage)
{
    health = health - damage;
    health = std::max(health, 0);
}

int Player::GetHealth() const
{
    return health;
}

bool Player::IsAlive() const
{
    return health > 0;
}

void Player::Draw() const
{
    const int barWidth = 300;
    const int barHeight = 35;
    int screenWidth = GetScreenWidth();
    const int barX = screenWidth - barWidth - 50;
    const int barY = 50;
    
    DrawRectangle(barX, barY, barWidth, barHeight, RED);
    
    int currentHealthWidth = static_cast<int>((static_cast<float>(health) / maxHealth) * barWidth);
    DrawRectangle(barX, barY, currentHealthWidth, barHeight, GREEN);
    
    std::string healthText = "Health: " + std::to_string(health) + "/" + std::to_string(maxHealth);
    DrawText(healthText.c_str(), barX + 55, barY + 6, 23, BLACK);
    
    if (!IsAlive())
    {
        int screenHeight = GetScreenHeight();
        
        const char* gameOverText = "GAME OVER";
        const char* restartText = "Press 'R' to restart";
        
        int gameOverTextWidth = MeasureText(gameOverText, 60);
        int restartTextWidth = MeasureText(restartText, 30);
        
        DrawRectangle(0, 0, screenWidth, screenHeight, ColorAlpha(BLACK, 0.7f));
        
        DrawText(gameOverText, (screenWidth - gameOverTextWidth) / 2, screenHeight / 2 - 40, 60, RED);
        DrawText(restartText, (screenWidth - restartTextWidth) / 2, screenHeight / 2 + 40, 30, WHITE);
    }
}