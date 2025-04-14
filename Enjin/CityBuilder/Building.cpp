#include "Building.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../C.hpp"


Building::Building(sf::Vector2i spawnPos, int size, sf::Color colour, Material cost, Material production) : pos(spawnPos), size(size), cost(cost), production(production)
{
    sprite = new sf::RectangleShape({C::GRID_SIZE, C::GRID_SIZE * 2.0f});
    sprite->setFillColor(colour);
    sprite->setOrigin(C::GRID_SIZE * 0.5f, C::GRID_SIZE * 2.0f);
    SyncPos();
}

void Building::Update(double dt)
{
    SyncPos();
}

void Building::Draw(sf::RenderWindow& win)
{
    win.draw(*sprite);
}

void Building::SetPostion(int x, int y)
{
    pos.x = x;
    pos.y = y;
}

sf::Vector2i Building::GetPosition()
{
    return pos;
}

void Building::SyncPos()
{
    sf::Vector2f p = {static_cast<float>(pos.x * C::GRID_SIZE), static_cast<float>(pos.y * C::GRID_SIZE)};
    sprite->setPosition(p);
}
