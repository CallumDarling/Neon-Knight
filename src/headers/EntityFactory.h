#ifndef ENTFAC
#define ENTFAC

#include "../components/Components.h"
#include "../entt/entt.hpp"
#include "ResourceHandler.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "EntityConfig.h"

class EntityFactory{
    public:
        entt::entity createPlayer(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text,sf::Vector2f loc);
        entt::entity createEnemy(entt::registry &registry);
        entt::entity createBrute(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path);
        entt::entity createHench(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path);
        entt::entity createGun(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, int angle);
        entt::entity createBoss(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path);

        entt::entity createUI(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, int partNo);
        entt::entity createLevelPart(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location);
        entt::entity createLadder(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location);
        entt::entity createBlock(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location);
        entt::entity createPlatform(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location);
        entt::entity createDoor(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location);
        entt::entity createBullet(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, float velocity);
        entt::entity createPlayerSword(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location);
        entt::entity createImage(entt::registry &registry, sf::Vector2f location, sf::Texture &texture, bool originCenter);
        entt::entity createText(entt::registry &registry, ResourceHandler<sf::Font, Fonts::ID> &fonts, sf::Vector2f location, std::string text, int size);
        entt::entity createRectangle(entt::registry &registry, sf::Vector2f location, sf::Vector2f dimensions, sf::Color color, bool fill);
        
};

#endif