﻿#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Entity.h"

struct Bullet
{
    int cx;
    int cy;

    float rx;
    float ry;

    bool aimingRight;

    bool destroy = false;

    sf::CircleShape* sprite;

    ~Bullet()
    {
        delete sprite;
    }

    void Initialise(int cx, int cy, float rx, float ry, bool aimingRight, sf::CircleShape* sprite)
    {
        this->cx = cx;
        this->cy = cy;

        this->rx = rx;
        this->ry = ry;

        this->aimingRight = aimingRight;

        this->sprite = sprite;
    }

    void syncPos()
    {
        sf::Vector2f pos = {(cx+rx) * C::GRID_SIZE, (cy+ry) * C::GRID_SIZE};

        sprite->setPosition(pos);
    }
};

class Player : public Entity
{

private:
    bool wasPressedCrouch = false;
    bool wasPressedLaserMouse = false;
    bool wasPressedShootMouse = false;
    bool wasPressedLaserController = false;
    bool wasPressedShootController = false;

    float laserAnimSpeed = 1500.0f;
    float laserLength = 0;
    bool firingLaser = false;
    int laserRange = 20;
    float laserPixelSize = 8.0f;
    
    bool showMuzzle = false;
    float muzzleLifetime = 0.1f;
    float muzzleLifetimeTimer = 0.0f;

    bool canShoot = true;
    float shootCooldown = 0.2f;
    float shootCooldownTimer = 0;
    float bulletSpeed = 100.0f;
    float bulletSize = 5.0f;
    std::vector<Bullet*> bullets;
    std::vector<Bullet*> bulletsToDestroy;
    
    sf::CircleShape* muzzleFireSprite;

    sf::RectangleShape* laserSprite = new sf::RectangleShape({0.0f, 0.0f});

    
public:

    
    
private:
    void PollControllerInput(double dt);

    void Shoot(double dt);
    
    void fireLaser(double dt);
    void drawAnimLaser(float x0, float x1, int y, double dt);
    void createLaserSprite(float length, int y);
    void createLaser(int length);

    
public:
    Player(sf::RectangleShape* standSprite, sf::RectangleShape* crouchSprite);
    
    void ProcessInput(sf::Event ev);
    void PollInput(double dt);

    void update(double dt) override;
    void draw(sf::RenderWindow& win) override;
};
