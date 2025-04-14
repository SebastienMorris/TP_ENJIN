#include "PowerPlant.h"

PowerPlant::PowerPlant(sf::Vector2i spawnPos) : Building(spawnPos, 5, Material(Materials::MONEY, 1), Material(Materials::ELECTRICITY, 1))
{
}

void PowerPlant::Update(double dt)
{
    Building::Update(dt);
}

void PowerPlant::Draw(sf::RenderWindow& win)
{
    Building::Draw(win);
}
