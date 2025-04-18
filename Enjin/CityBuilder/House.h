#pragma once
#include "Building.h"

class House : Building
{
public:
    House();

    void Update(double dt) override;
    void Draw(sf::RenderWindow& win) override;
};
