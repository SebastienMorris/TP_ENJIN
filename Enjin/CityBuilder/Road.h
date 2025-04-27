#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}

class Road
{
    sf::RectangleShape* sprite;

    sf::Vector2i pos;

    bool confirmed = false;
    
public:
    Road(sf::Vector2i spawnPos);

    void Update(double dt);
    void Draw(sf::RenderWindow& win);

    void Confirm();

    sf::Vector2i GetPosition() const {return pos;}
    void SetPosition(int x, int y) {pos = {x, y}; SyncPos();}

    void SetOutlineColour(sf::Color colour) const {sprite->setOutlineColor(colour);}
    void SetOutline(bool show) const {sprite->setOutlineThickness(show ? 2 : 0);}

private:
    void SyncPos();
};
