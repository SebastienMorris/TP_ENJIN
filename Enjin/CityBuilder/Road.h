#pragma once
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RectangleShape;
    class RenderWindow;
}
}

class Road
{
    sf::RectangleShape* sprite;

    sf::Vector2i pos;
    
public:
    Road(sf::Vector2i spawnPos);

    void Update(double dt);
    void Draw(sf::RenderWindow& win);

    sf::Vector2i GetPosition() const {return pos;}
};
