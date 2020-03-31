#ifndef ENTITY
#define ENTITY


#include <SFML/Graphics.hpp>
#include "ResourceHandler.h"
#include <iostream>

class Entity{
    public:
        Entity(sf::Texture& tex, float posX, float posY, bool grav);
        void draw(sf::RenderTarget& target);
        void setVelocity(sf::Vector2f velocity);
        void setVelocity(float vx, float vy);
        void setPostion(sf::Vector2f velocity);
        void setPostion(float vx, float vy);
        bool getGrav();
        void move();
        void scale(float x, float y);
        bool getFacing();
        void setFacing(bool facingRight);
        
        void setOrigin(float x, float y);
        sf::FloatRect getBoundingBox();
        sf::Vector2f getVelocity() const;
        sf::Vector2f getPosition();

    private:
        sf::Sprite mSprite;
        sf::Vector2f mVelocity;
        bool hasGrav;
        bool facingRight = true;

        
};

#endif
