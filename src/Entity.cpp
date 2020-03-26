#include "headers/Entity.h"
#include <SFML/Graphics.hpp>


Entity::Entity(sf::Texture& tex, float posX, float posY, bool grav){
    mSprite.setTexture(tex);
    hasGrav = grav;
    setPostion(sf::Vector2f(posX,posY));
}

bool Entity::getGrav(){
    return hasGrav;
}

void Entity::scale(float x, float y){
    mSprite.scale(x, y);
}

void Entity::move(){
    mSprite.move(mVelocity);
    
}




void Entity::draw(sf::RenderTarget& target){
    target.draw(mSprite);
}

//get & set



sf::Vector2f Entity::getVelocity() const{
    return mVelocity;
    
}

sf::FloatRect Entity::getBoundingBox(){
    return mSprite.getGlobalBounds();
}

void Entity::setVelocity(sf::Vector2f velocity){
    mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy){
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Entity::getPosition(){
    return mSprite.getPosition();
}

void Entity::setPostion(sf::Vector2f pos){
    mSprite.setPosition(pos);
}

void Entity::setPostion(float x, float y){
    mSprite.setPosition(x,y);
}