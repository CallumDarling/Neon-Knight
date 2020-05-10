#include "../components/Components.h"
#include "../entt/entt.hpp"
#include "ResourceHandler.h"
#include <SFML/Graphics.hpp>

entt::entity createPlayer(entt::registry &registry, std::string textFilename){
    //declare variables needed for Draw component
    ResourceHandler<sf::Texture, Textures::ID> text;
    try{
        text.load(Textures::Player, "media/textures/att2.png");
    }catch(const std::exception& e){
    }
    sf::Vector2f loc;
    loc.x = 0.0;
    loc.y = 0.0;
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Player));
    sprit.setPosition(loc);
    
    //creates entity and adds components
    entt::entity e = registry.create();
    registry.emplace<Draw>(e, text.get(Textures::Player),sprit,true,loc);
}
