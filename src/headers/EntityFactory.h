#include "../components/Components.h"
#include "../entt/entt.hpp"
#include "ResourceHandler.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class EntityFactory{
    public:
        entt::entity createPlayer(entt::registry &registry, std::string textFilename, ResourceHandler<sf::Texture, Textures::ID> &text);

};