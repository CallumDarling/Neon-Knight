#include "headers/Entity.h"
#include <SFML/Graphics.hpp>


void Entity::setVelocity(sf::Vector2f velocity){
    mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy){
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const{
    return mVelocity;
}

void draw(sf::RenderTarget& target, sf::RenderStates states){
    int i = 0;
}