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
        int loadLevel(std::string fileName,
        ResourceHandler<sf::Texture, Textures::ID>& textures
        ,std::vector<Entity>& entList
        ,entt::registry& reg);
    private:
        int i;

};

#endif