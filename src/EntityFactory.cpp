#include "headers/EntityFactory.h" 
 



entt::entity EntityFactory::createPlayer(entt::registry &registry, 
                                        ResourceHandler<sf::Texture,Textures::ID> &text,sf::Vector2f loc){
    // sf::Vector2f loc;
    // loc.x = 400.0;
    // loc.y = 400.0;
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Player));
    sprit.setPosition(loc);
    std::vector<std::string> cExc = {""};
    std::vector<int> animTextures = {1,13,18};
    sf::Vector2f vel = {1.f,1.f};
    //creates entity and adds components
    // TODO move these values to entityConfig
    entt::entity e = registry.create();
    registry.emplace<Draw>(e, text.get(Textures::Player),sprit,true,loc,true);
    registry.emplace<Physics>(e,true,1.f,true,cExc);
    registry.emplace<AttackMelee>(e,20,150);
    registry.emplace<AttackRanged>(e,"bullet",20,15);
    registry.emplace<Movement>(e, vel);
    registry.emplace<Animation>(e, animTextures, 15.f, 0.f);
    registry.emplace<Health>(e, 100, 100);
    return e;

}

entt::entity EntityFactory::createEnemy(entt::registry &registry){
    sf::Vector2f loc;
    std::vector<std::string> cExc = {""};
    //creates entity and adds components
    entt::entity e = registry.create();
    registry.emplace<Physics>(e,false,1.f,true,cExc);
    registry.emplace<Movement>(e, loc);
    return e;
}

entt::entity EntityFactory::createBrute(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path){
    entt::entity e = createEnemy(registry);
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Brute));
    location.y+=12;
    sprit.setPosition(location);
    std::vector<std::string> cExc = {""};
    std::vector<int> animTextures = {5,16};
    registry.emplace<Draw>(e, text.get(Textures::Brute),sprit,true,location,true,"enemy");
    registry.emplace<AttackMelee>(e,bruteAttack,bruteRange);
    registry.emplace<AI>(e, path, bruteVision);
    registry.emplace<Animation>(e, animTextures, bruteIdleTime, bruteTransitionTime);
    return e;
}

entt::entity EntityFactory::createHench(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path){
    entt::entity e = createEnemy(registry);
    sf::Sprite sprit;
    location.y+=16;
    sprit.setTexture(text.get(Textures::Henchman));
    sprit.setPosition(location);
    std::vector<std::string> cExc = {""};
    std::vector<int> animTextures = {2,14};
    registry.emplace<Draw>(e, text.get(Textures::Henchman),sprit,true,location,true,"enemy");
    registry.emplace<AttackMelee>(e,henchMelee,henchMeleeRange);
    registry.emplace<AttackRanged>(e,henchProj,henchProjRange,henchProjDamage);
    registry.emplace<AI>(e, path, henchVision);
    registry.emplace<Animation>(e, animTextures, henchIdleTime, henchTransitionTime);
    return e;
}
entt::entity EntityFactory::createGun(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, int angle){
    entt::entity e = createEnemy(registry);
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Gunman));
    sprit.setPosition(location);
    sf::IntRect ir = sprit.getTextureRect();
    sprit.setTextureRect(sf::IntRect(ir.width, 0, -ir.width, ir.height));
    std::vector<std::string> cExc = {""};
    std::vector<int> animTextures = {4,4};
    std::vector<sf::Vector2f> cO = {};
    registry.emplace<Draw>(e, text.get(Textures::Henchman),sprit,true,location,true,"enemy");
    registry.emplace<AttackRanged>(e,gunProj,gunProjRange,gunProjDamage);
    registry.emplace<AI>(e, cO, gunVision);
    registry.emplace<Animation>(e, animTextures, gunIdleTime, gunTransitionTime);
    return e;
}

entt::entity EntityFactory::createBoss(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, std::vector<sf::Vector2f> path){
    entt::entity e = createEnemy(registry);
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Boss));
    location.y+=12;
    sprit.setPosition(location);
    std::vector<std::string> cExc = {""};
    std::vector<int> animTextures = {3,17};
    registry.emplace<Draw>(e, text.get(Textures::Boss),sprit,true,location,true,"boss");
    registry.emplace<AttackMelee>(e,bossMelee,bossMeleeRange);
    registry.emplace<AttackRanged>(e,bossProj,bossProjRange,bossProjDamage);
    registry.emplace<AI>(e, path, bossVision);
    registry.emplace<Animation>(e, animTextures, bossIdleTime, bossTransitionTime);
    return e;
}

entt::entity EntityFactory::createLevelPart(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = registry.create();
    // registry.emplace<Movement>(e, location);
    return e;
}

entt::entity EntityFactory::createUI(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, int partNo){
    entt::entity e = createLevelPart(registry, text, location);
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::HealthBar));
    sprit.setPosition(location);
    //TODO Add part number and UI Elements, animation for these?
    registry.emplace<Draw>(e, text.get(Textures::HealthBar),sprit,true,location,true);    
    // registry.emplace<Animation>(e, cExc, bossIdleTime, bossTransitionTime);
    return e;
}

entt::entity EntityFactory::createLadder(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = createLevelPart(registry, text, location);
    std::vector<std::string> cExc = {""};
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Ladder));
    sprit.setPosition(location);
    registry.emplace<Draw>(e, text.get(Textures::Ladder),sprit,true,location,true);  
    registry.emplace<Physics>(e,true,1.f,true,cExc);
    return e; 
}

entt::entity EntityFactory::createBlock(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = createLevelPart(registry, text, location);
    std::vector<std::string> cExc = {""};
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Block));
    sprit.setPosition(location);
    registry.emplace<Draw>(e, text.get(Textures::Block),sprit,true,location,true);
    registry.emplace<Physics>(e,false,1.f,true,cExc);
    return e; 
}

entt::entity EntityFactory::createPlatform(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = createLevelPart(registry, text, location);
    std::vector<std::string> cExc = {""};
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Platform));
    sprit.setPosition(location);
    registry.emplace<Draw>(e, text.get(Textures::Platform),sprit,true,location,true);
    registry.emplace<Physics>(e,false,1.f,true,cExc);
    return e; 
}

entt::entity EntityFactory::createDoor(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = createLevelPart(registry, text, location);
    std::vector<std::string> cExc = {""};
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Door));
    sprit.setPosition(location);
    registry.emplace<Draw>(e, text.get(Textures::Door),sprit,true,location,true);
    registry.emplace<Physics>(e,false,1.f,false,cExc);
    return e; 
}

entt::entity EntityFactory::createBullet(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location, float velocity){
        entt::entity e = createEnemy(registry);
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Bullet));
    sprit.setPosition(location);
    registry.emplace<Draw>(e, text.get(Textures::Bullet),sprit,true,location,true,"bullet");
    sf::Vector2f vel = {velocity,0.f};
    registry.emplace_or_replace<Movement>(e,vel);

    return e;
}

entt::entity EntityFactory::createPlayerSword(entt::registry &registry, ResourceHandler<sf::Texture, Textures::ID> &text, sf::Vector2f location){
    entt::entity e = createLevelPart(registry, text, location);
    std::vector<std::string> cExc = {""};
    sf::Sprite sprit;
    sprit.setTexture(text.get(Textures::Sword));
    sprit.setPosition(location);
    registry.emplace<Draw>(e, text.get(Textures::Door),sprit,false,location,true,"sword");
    registry.emplace<Physics>(e,false,1.f,false,cExc);
    return e;
}


entt::entity EntityFactory::createText(entt::registry &registry, ResourceHandler<sf::Font, Fonts::ID> &fonts, sf::Vector2f location, std::string text, int size, bool fromCenter){
    entt::entity e = registry.create();
    sf::Text tex;
    tex.setFont(fonts.get(Fonts::MenuFont));
    tex.setString(text);
    tex.setCharacterSize(size);
    tex.setStyle(sf::Text::Bold);
    if(fromCenter){
        tex.setOrigin(tex.getGlobalBounds().width/2,tex.getGlobalBounds().height/2);
    }
    tex.setPosition(location);
    tex.setOutlineColor(sf::Color::Black);
    tex.setOutlineThickness(2);
    registry.emplace<Text>(e, tex);
    return e;
}

entt::entity EntityFactory::createImage(entt::registry &registry, sf::Vector2f location, sf::Texture &texture, bool originCenter){
    entt::entity e = registry.create();
    std::vector<std::string> cExc = {""};
    sf::Sprite sprit;
    sprit.setTexture(texture);
    if(originCenter){
        sprit.setOrigin(sprit.getGlobalBounds().width/2, sprit.getGlobalBounds().height/2);
    }
    sprit.setPosition(location);
    registry.emplace<Draw>(e,texture,sprit,true,location,true);
    return e;
}

entt::entity EntityFactory::createRectangle(entt::registry &registry, sf::Vector2f location, sf::Vector2f dimensions, sf::Color color, bool fill){
    entt::entity e = registry.create();
    sf::RectangleShape rect;
    rect.setPosition(location);
    rect.setSize(dimensions);
    rect.setOutlineColor(color);
    rect.setOutlineThickness(1);
    if(fill){
        rect.setFillColor(color);
    }else{
        rect.setFillColor(sf::Color::Transparent);
    }
    registry.emplace<DrawShape>(e, rect, true, location);
    return e;
}