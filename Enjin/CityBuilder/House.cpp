#include "House.h"

#include <SFML/Graphics/Color.hpp>

#include "../Game.hpp"

House::House() : Building(3, sf::Color::Blue, Material(Materials::WOOD, 3), Material(Materials::ELECTRICITY, 1), Material(MONEY, 1))
{
    SetProductionState(true, 5.0f);
}

void House::Update(double dt)
{
    Building::Update(dt);
}

void House::Draw(sf::RenderWindow& win)
{
    Building::Draw(win);
}

void House::Confirm()
{
    Building::Confirm();
    Game* g = Game::me;
    g->AddInhabitants(1);
}


