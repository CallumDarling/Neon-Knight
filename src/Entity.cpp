#include "headers/Entity.h"
#include <SFML/Graphics.hpp>

Entity::Entity(sf::Texture& tex){
    mSprite.setTexture(tex);
}

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

void Entity::draw(sf::RenderTarget& target){
    target.draw(mSprite);
}