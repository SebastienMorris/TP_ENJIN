#include "House.h"

House::House(sf::Vector2i spawnPos) : Building(spawnPos, 3, Material(Materials::WOOD, 1), Material(MONEY, 1))
{
    
}

void House::Update(double dt)
{
    Building::Update(dt);
}

void House::Draw(sf::RenderWindow& win)
{
    Building::Draw(win);
}
