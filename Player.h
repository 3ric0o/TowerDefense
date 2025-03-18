#pragma once
#include "raylib.h"

class Player
{
public:
    Player(int initialHealth = 100);
    
    void TakeDamage(int damage);
    int GetHealth() const;
    bool IsAlive() const;
    
    void Draw() const;
    
private:
    int health;
    int maxHealth;
};