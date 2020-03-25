#include "headers/Entity.h"
#include <SFML/Graphics.hpp>


Entity::Entity(sf::Texture& tex){
    mSprite.setTexture(tex);
    std::cout << tex.getSize().x+" : "+ tex.getSize().y; //<< std::endl;
}

void Entity::move(){
    mSprite.move(mVelocity);
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