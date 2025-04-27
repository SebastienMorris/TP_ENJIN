#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "../C.hpp"

namespace sf
{
    class Color;
    class RectangleShape;
    class RenderWindow;
}

enum Materials
{
    MONEY,
    WOOD,
    ELECTRICITY
};

struct Material
{
    Materials type;
    int amount;

    Material(const Materials t, const int a) : type(t), amount(a){}
};

template<typename T> class BuildingType
{
public:
    static T* Allocate() {return new T;} 
};

class Building
{
    sf::Vector2i pos;
    
    int size;
    
    Material cost;
    Material production;

    sf::RectangleShape* sprite;

    bool confirmed = false;
    bool recurringProduction = false;

    float productionCooldown = 0.0f;
    float productionTimer = 0.0f;
    
public:
    Building(int size, sf::Color colour, Material cost, Material production);

    virtual void Update(double dt);
    virtual void Draw(sf::RenderWindow& win);

    virtual void Confirm();
    void Produce();

    void SetPosition(int x, int y) {pos.x = x; pos.y = y; SyncPos();}
    sf::Vector2i GetPosition() const {return pos;}
    int GetSize() const {return size;}

    void SetProductionState(bool recurring, float cooldown);

    const Material& GetCost() const{ return cost;}
    
    void SetOutlineColour(sf::Color colour) const {sprite->setOutlineColor(colour);}
    void SetOutline(bool show) const {sprite->setOutlineThickness(show ? 2 : 0);}

private:
    void SyncPos();
};
