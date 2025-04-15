#include "Player.h"

#include <SFML/Window/Event.hpp>

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
