#include "headers/EntityFactory.h" 
 
entt::entity EntityFactory::createPlayer(entt::registry &registry, 
                                        ResourceHandler<sf::Texture,Textures::ID> &text){
    sf::Vector2f loc;
    loc.x = 400.0;
    loc.y = 400.0;
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Player));
    sprit.setPosition(loc);
    std::vector<std::string> cExc = {""};
    
    //creates entity and adds components
    // TODO move these values to entityConfig
    entt::entity e = registry.create();
    registry.emplace<Draw>(e, text.get(Textures::Player),sprit,true,loc);
    registry.emplace<Physics>(e,true,1.f,true,cExc);
    registry.emplace<AttackMelee>(e,20,150);
    registry.emplace<AttackRanged>(e,"bullet",20,15);
    registry.emplace<Movement>(e, loc);
    registry.emplace<Animation>(e, cExc, 15.f, 0.f);
    registry.emplace<Health>(e, 100, 100);
    return e;

}

entt::entity EntityFactory::createEnemy(entt::registry &registry){
    sf::Vector2f loc;
    std::vector<std::string> cExc = {""};
    //creates entity and adds components
    entt::entity e = registry.create();
    registry.emplace<Physics>(e,true,1.f,true,cExc);
    registry.emplace<Movement>(e, loc);
    return e;
}

entt::entity EntityFactory::createBrute(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path){
    entt::entity e = createEnemy(registry);
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Henchman));
    sprit.setPosition(location);
    std::vector<std::string> cExc = {""};
    registry.emplace<Draw>(e, text.get(Textures::Henchman),sprit,true,location);
    registry.emplace<AttackMelee>(e,bruteAttack,bruteRange);
    registry.emplace<AI>(e, path, bruteVision);
    registry.emplace<Animation>(e, cExc, bruteIdleTime, bruteTransitionTime);
    return e;
}

entt::entity EntityFactory::createHench(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path){
    entt::entity e = createEnemy(registry);
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Henchman));
    sprit.setPosition(location);
    std::vector<std::string> cExc = {""};
    registry.emplace<Draw>(e, text.get(Textures::Henchman),sprit,true,location);
    registry.emplace<AttackMelee>(e,henchMelee,henchMeleeRange);
    registry.emplace<AttackRanged>(e,henchProj,henchProjRange,henchProjDamage);
    registry.emplace<AI>(e, path, henchVision);
    registry.emplace<Animation>(e, cExc, henchIdleTime, henchTransitionTime);
    return e;
}
entt::entity EntityFactory::createGun(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, int angle){
    entt::entity e = createEnemy(registry);
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Henchman));
    sprit.setPosition(location);
    std::vector<std::string> cExc = {""};
    std::vector<sf::Vector2f> cO = {};
    registry.emplace<Draw>(e, text.get(Textures::Henchman),sprit,true,location);
    registry.emplace<AttackRanged>(e,gunProj,gunProjRange,gunProjDamage);
    registry.emplace<AI>(e, cO, gunVision);
    registry.emplace<Animation>(e, cExc, gunIdleTime, gunTransitionTime);
    return e;
}

entt::entity EntityFactory::createBoss(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path){
    entt::entity e = createEnemy(registry);
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Boss));
    sprit.setPosition(location);
    std::vector<std::string> cExc = {""};
    registry.emplace<Draw>(e, text.get(Textures::Boss),sprit,true,location);
    registry.emplace<AttackMelee>(e,bossMelee,bossMeleeRange);
    registry.emplace<AttackRanged>(e,bossProj,bossProjRange,bossProjDamage);
    registry.emplace<AI>(e, path, bossVision);
    registry.emplace<Animation>(e, cExc, bossIdleTime, bossTransitionTime);
    return e;
}

entt::entity EntityFactory::createLevelPart(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = registry.create();
    registry.emplace<Movement>(e, location);
    return e;
}

entt::entity EntityFactory::createUI(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, int partNo){
    entt::entity e = createLevelPart(registry, text, location);
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::HealthBar));
    sprit.setPosition(location);
    //TODO Add part number and UI Elements, animation for these?
    registry.emplace<Draw>(e, text.get(Textures::HealthBar),sprit,true,location);    
    // registry.emplace<Animation>(e, cExc, bossIdleTime, bossTransitionTime);
    return e;
}

entt::entity EntityFactory::createLadder(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = createLevelPart(registry, text, location);
    std::vector<std::string> cExc = {""};
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Ladder));
    sprit.setPosition(location);
    registry.emplace<Draw>(e, text.get(Textures::Ladder),sprit,true,location);  
    registry.emplace<Physics>(e,true,1.f,true,cExc);
    return e; 
}

entt::entity EntityFactory::createBlock(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = createLevelPart(registry, text, location);
    std::vector<std::string> cExc = {""};
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Block));
    sprit.setPosition(location);
    registry.emplace<Draw>(e, text.get(Textures::Block),sprit,true,location);
    registry.emplace<Physics>(e,false,1.f,true,cExc);
    return e; 
}

entt::entity EntityFactory::createPlatform(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = createLevelPart(registry, text, location);
    std::vector<std::string> cExc = {""};
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Platform));
    sprit.setPosition(location);
    registry.emplace<Draw>(e, text.get(Textures::Platform),sprit,true,location);
    registry.emplace<Physics>(e,false,1.f,true,cExc);
    return e; 
}

entt::entity EntityFactory::createDoor(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = createLevelPart(registry, text, location);
    std::vector<std::string> cExc = {""};
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Door));
    sprit.setPosition(location);
    registry.emplace<Draw>(e, text.get(Textures::Door),sprit,true,location);
    registry.emplace<Physics>(e,false,1.f,false,cExc);
    return e; 
}

entt::entity EntityFactory::createBullet(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = createLevelPart(registry, text, location);
    return e;
}