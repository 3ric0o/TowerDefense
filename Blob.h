#pragma once
#include "Enemy.h"
#include "raylib.h"

class Blob : public Enemy {
public:
    Blob(float x, float y);
    
    void Draw() const override;
    std::string GetType() const override;
    
private:
    void LoadAnimations() override;
};