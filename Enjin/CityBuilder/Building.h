#pragma once
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class Color;
}

namespace sf
{
    class RectangleShape;
}

namespace sf
{
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

    Material(const Materials t, const int a) : type(t), amount(a)
    {
        
    }
};

class Building
{
    sf::Vector2i pos;
    
    int size;
    
    Material cost;
    Material production;

    sf::RectangleShape* sprite;

    
    bool recurringProduction = false;

    float productionCooldown = 0.0f;
    float productionTimer = 0.0f;
    
public:
    Building(sf::Vector2i spawnPos, int size, sf::Color colour, Material cost, Material production);

    virtual void Update(double dt);
    virtual void Draw(sf::RenderWindow& win);
    void Produce();

    void SetPostion(int x, int y);
    sf::Vector2i GetPosition();

    void SetProductionState(bool recurring, float cooldown);
    
private:
    void SyncPos();
};
