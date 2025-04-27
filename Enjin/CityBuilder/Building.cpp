#include "Building.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../C.hpp"
#include "../Game.hpp"


Building::Building(int size, sf::Color colour, Material cost, Material production) : pos({0,0}), size(size), cost(cost), production(production)
{
    sprite = new sf::RectangleShape({static_cast<float>(C::GRID_SIZE * size), static_cast<float>(C::GRID_SIZE * size)});
    sprite->setFillColor(colour);
    sprite->setOrigin(C::GRID_SIZE * (size / 2.0f), C::GRID_SIZE * (size / 2.0f));
   
    SyncPos();
}

void Building::Update(double dt)
{
    if(confirmed)
    {
        if(recurringProduction)
        {
            productionTimer -= dt;
            if(productionTimer <= 0.0f)
            {
                Produce();
                productionTimer = productionCooldown;
            }
        }
    }
    
    SyncPos();
}

void Building::Draw(sf::RenderWindow& win)
{
    win.draw(*sprite);
}

void Building::Confirm()
{
    confirmed = true;
    SetOutline(false);
}

void Building::Produce()
{
    Game* me = Game::me;

    me->ProduceMaterial(production);
}

void Building::SetProductionState(bool recurring, float cooldown)
{
    recurringProduction = recurring;
    productionCooldown = recurringProduction ? cooldown : 0.0f;
}

void Building::SyncPos()
{
    sf::Vector2f p = {static_cast<float>(pos.x * C::GRID_SIZE), static_cast<float>(pos.y * C::GRID_SIZE)};
    sprite->setPosition(p);
}
