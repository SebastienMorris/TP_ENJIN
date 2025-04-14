#include "Factory.h"

Factory::Factory(sf::Vector2i spawnPos) : Building(spawnPos, 5, Material(Materials::MONEY, 1), Material(Materials::WOOD, 1))
{
}

void Factory::Update(double dt)
{
    Building::Update(dt);
}

void Factory::Draw(sf::RenderWindow& win)
{
    Building::Draw(win);
}
