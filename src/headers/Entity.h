#ifndef ENTITY
#define ENTITY


#include <SFML/Graphics.hpp>
#include "ResourceHandler.h"
#include <iostream>

class Entity{
    public:
        Entity(sf::Texture& tex, float posX, float posY);
        void draw(sf::RenderTarget& target);
        void setVelocity(sf::Vector2f velocity);
        void setVelocity(float vx, float vy);
        void setPostion(sf::Vector2f velocity);
        void setPostion(float vx, float vy);
        
        void move();
 
        sf::FloatRect getBoundingBox();
        sf::Vector2f getVelocity() const;
        sf::Vector2f getPosition();
    private:
        sf::Sprite mSprite;
        sf::Vector2f mVelocity;
        
};

#endif
