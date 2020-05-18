#ifndef LEVELHANDLE
#define LEVELHANDLE

#include <SFML/Graphics.hpp>
#include "ResourceHandler.h"
#include <iostream>
#include <fstream>
#include "Entity.h"
#include <sstream>
#include "../entt/entt.hpp"
#include "EntityFactory.h"


class LevelHandler{
    public:
        LevelHandler();
        entt::entity loadLevel(std::string fileName,
                            ResourceHandler<sf::Texture, Textures::ID>& textures,
                            ResourceHandler<sf::Font, Fonts::ID>& fonts,
                            std::vector<Entity>& entList,
                            entt::registry& reg);
        int saveLevel(std::string fileName, std::map<std::string, int> designMap);
    private:
        int i;

};

#endif