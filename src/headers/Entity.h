#ifndef ENTITY
#define ENTITY


#include <SFML/Graphics.hpp>
#include "ResourceHandler.h"

class Entity{
    public:
        Entity(sf::Texture& tex);
        void draw(sf::RenderTarget& target);
        void setVelocity(sf::Vector2f velocity);
        void setVelocity(float vx, float vy);
 
        sf::Vector2f getVelocity() const;
    private:
        sf::Sprite mSprite;
        sf::Vector2f mVelocity;
        
};

#endif
