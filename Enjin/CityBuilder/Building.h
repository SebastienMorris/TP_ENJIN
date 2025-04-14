#pragma once
#include <SFML/System/Vector2.hpp>

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
    
public:
    Building(sf::Vector2i spawnPos, int size, Material cost, Material production);

    virtual void Update(double dt);
    virtual void Draw(sf::RenderWindow& win);

    void SetPostion(int x, int y);
    sf::Vector2i GetPosition();

private:
    void SyncPos();
};
