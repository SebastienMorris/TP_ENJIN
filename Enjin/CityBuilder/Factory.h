#pragma once
#include <SFML/Graphics/Drawable.hpp>

#include "Building.h"

class Factory : Building
{
public:
    Factory(sf::Vector2i spawnPos);

    void Update(double dt) override;
    void Draw(sf::RenderWindow& win) override;
};
