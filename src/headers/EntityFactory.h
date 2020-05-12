#include "../components/Components.h"
#include "../entt/entt.hpp"
#include "ResourceHandler.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "EntityConfig.h"

class EntityFactory{
    public:
        entt::entity createPlayer(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text);
        entt::entity createEnemy(entt::registry &registry);
        entt::entity createBrute(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path);
        entt::entity createHench(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path);
        entt::entity createGun(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, int angle);
        entt::entity createBoss(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path);

};