#pragma once
#include "Building.h"

class House : Building
{
public:
    House(sf::Vector2i spawnPos);

    void Update(double dt) override;
    void Draw(sf::RenderWindow& win) override;
};
