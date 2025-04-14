#pragma once

namespace sf
{
    class Event;
}

class Player
{
public:
    void ProcessInput(sf::Event ev);
    void Update(double dt);
};
