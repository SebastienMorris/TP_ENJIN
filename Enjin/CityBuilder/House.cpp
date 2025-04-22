#include "House.h"

#include <SFML/Graphics/Color.hpp>

#include "../Game.hpp"

House::House() : Building(3, sf::Color::Blue, Material(Materials::WOOD, 1), Material(MONEY, 1))
{
    SetProductionState(true, 5.0f);
    Game* g = Game::me;
    g->AddInhabitants(1);
}

void House::Update(double dt)
{
    Building::Update(dt);
}

void House::Draw(sf::RenderWindow& win)
{
    Building::Draw(win);
}
