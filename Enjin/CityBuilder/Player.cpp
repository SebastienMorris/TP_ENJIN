#include "Player.h"

#include <SFML/Window/Event.hpp>

#include "House.h"
#include "../C.hpp"
#include "../Game.hpp"

class Game;

Player::Player()
{
}

Player::~Player()
{
    delete money;
    delete wood;
}

void Player::ProcessInput(sf::Event ev)
{
    Game* g = Game::me; 
    
    if(ev.mouseButton.button == sf::Mouse::Right)
    {
        g->TryPlaceBuilding(ev.mouseButton.x / C::GRID_SIZE, ev.mouseButton.y / C::GRID_SIZE, (Building*)BuildingType<House>::Allocate());
    }
}

void Player::Update(double dt)
{
}

void Player::AddMoney(int amount)
{
    money->amount += amount;
}

void Player::AddWood(int amount)
{
    wood->amount += amount;
}

void Player::AddElectricity(int amount)
{
    electricity->amount += amount;
}
