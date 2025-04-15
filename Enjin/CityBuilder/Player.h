#pragma once
#include "Building.h"

namespace sf
{
    class Event;
}

class Player
{
    Material* money = new Material(Materials::MONEY, 0);
    Material* wood = new Material(Materials::WOOD, 0);
    Material* electricity = new Material(Materials::ELECTRICITY, 0);
    
public:
    Player();
    ~Player();
    
    void ProcessInput(sf::Event ev);
    void Update(double dt);

    void AddMoney(int amount);
    void AddWood(int amount);
    void AddElectricity(int amount);
};
