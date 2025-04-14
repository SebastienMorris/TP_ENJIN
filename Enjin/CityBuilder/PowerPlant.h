#pragma once
#include <SFML/Graphics/Drawable.hpp>

#include "Building.h"

class PowerPlant : Building
{
public:
    PowerPlant(sf::Vector2i spawnPos);

    void Update(double dt) override;
    void Draw(sf::RenderWindow& win) override;
};
