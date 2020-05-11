#include "headers/EntityFactory.h" 
 
entt::entity EntityFactory::createPlayer(entt::registry &registry, std::string textFilename, 
                                        ResourceHandler<sf::Texture, Textures::ID> &text){
    //declare variables needed for Draw component
    // try{
    //     text.load(Textures::Player, "media/textures/att2.png");
    // }catch(const std::exception& e){
    //     std::cout << "TextureBroke" << std::endl;
    // }
    sf::Vector2f loc;
    loc.x = 400.0;
    loc.y = 400.0;
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Player));
    sprit.setPosition(loc);
    std::vector<std::string> cExc = {""};
    
    //creates entity and adds components
    entt::entity e = registry.create();
    registry.emplace<Draw>(e, text.get(Textures::Player),sprit,true,loc);
    registry.emplace<Physics>(e,true,1.f,true,cExc);
    registry.emplace<AttackMelee>(e,20,150);
    registry.emplace<AttackRanged>(e,"bullet",20,15);
    registry.emplace<Movement>(e, loc);
    registry.emplace<Animation>(e, cExc, 15.f, 0.f);
    return e;

}