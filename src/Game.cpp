#include "headers/Game.h"

Game::Game() : worldView(sf::FloatRect(32.f, 32.f, 768.f, 432.f)),
               mWindow(sf::VideoMode(1600, 900), "SFML Application", sf::Style::Fullscreen) {
    mWindow.setKeyRepeatEnabled(true);
    mWindow.setVerticalSyncEnabled(true);
    mWindow.setFramerateLimit(60);
}

void Game::loadLevel(std::string level){
    registry.clear();
    bossHealth = 50;
    EntityFactory eFac;
    LevelHandler lvlH;
    if(level.compare("menu")==0){
        // std::cout << "compare menu" << std::endl;
        // playerID = eFac.createPlayer(registry, textures,{400.f,400.f});
        menuSection = 0;
        menuList.clear();
        menuList.push_back(makeMenuButton({250.f,350.f}, 15,2,"PLAY",true));
        menuList.push_back(makeMenuButton({250.f,425.f}, 15,2,"LEVEL DESIGN",false));
        menuList.push_back(makeMenuButton({250.f,500.f}, 15,2,"EXIT",false));
        logo = eFac.createImage(registry,{400,275}, textures.get(Textures::Logo), true);
        registry.get<Draw>(logo).sprite.scale(0.35,0.35);
    }else
    {
        // playerID = eFac.createPlayer(registry, textures,{0.f,0.f});
    }
    
    currentLevel = level;
    entt::entity e = lvlH.loadLevel("levels/"+level, textures, fonts, entList, registry);
    // std::cout << "level loaded" << std::endl;
    if(registry.valid(e)){
        // std::cout << "e valid" << std::endl;
        // registry.replace(playerID, e);
        playerID = e;
    }

    if(!registry.valid(playerSword)){
        const auto swordView = registry.view<Draw>();
        for (const entt::entity s : swordView) {
            if(swordView.get<Draw>(s).label == "sword"){
                swordView.get<Draw>(s).visible = false;
                playerSword = s;
                // std::cout << "sword loaded" << std::endl;
            }
        }
    }
}



bool Game::initTextures(std::vector<Entity> &eList) {
    try {
        textures.load(Textures::Player, "media/textures/att2.png");
        textures.load(Textures::Henchman, "media/textures/hench1.png");
        textures.load(Textures::Boss, "media/textures/Boss1.png");
        textures.load(Textures::Block, "media/textures/basicblock.png");
        textures.load(Textures::Landscape, "media/textures/bg.jpg");
        textures.load(Textures::HealthBar, "media/textures/healthbar.png");
        textures.load(Textures::Ladder, "media/textures/ladder.png");
        textures.load(Textures::Platform, "media/textures/platform.png");
        textures.load(Textures::Door, "media/textures/door.png");
        textures.load(Textures::Bullet, "media/textures/bullet.png");
        textures.load(Textures::Logo, "media/textures/logo.png");
        textures.load(Textures::Gunman, "media/textures/Gunman1.png");
        textures.load(Textures::Brute, "media/textures/Brute1.png");
        textures.load(Textures::PlayerW, "media/textures/nkwalk.png");
        textures.load(Textures::HenchW, "media/textures/henchw.png");
        textures.load(Textures::Sword, "media/textures/sword.png");
        textures.load(Textures::BruteW, "media/textures/BruteW.png");
        textures.load(Textures::BossW, "media/textures/BossW.png");
        textures.load(Textures::PlayerA, "media/textures/PlayerA.png");
        fonts.load(Fonts::MenuFont, "media/fonts/menufont.woff");
    } catch (const std::exception &e) {
        return 0;
    }
    loadLevel("menu");
    loadLevel("menu");
    return 1;
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code, false);
            break;
        case sf::Event::MouseButtonPressed:
            if(event.mouseButton.button==sf::Mouse::Button::Left){
                mouseLClicked = true;
            }else{
                mouseRClicked = true;
            }
            
            handleMouseInput(event.mouseButton.button, true);
            break;
        case sf::Event::MouseButtonReleased:
            if(event.mouseButton.button==sf::Mouse::Button::Left){
                mouseLClicked = false;
            }else{
                mouseRClicked = false;
            }
            handleMouseInput(event.mouseButton.button, false);
            break;
        case sf::Event::MouseMoved:
            handleMouseInput(event.mouseButton.button, false);
            break;
        case sf::Event::Closed:
            mWindow.close();
            break;
        }
    }
}


void Game::addBlockToEditor(sf::Vector2f coords){
    std::string sCoords = std::to_string(coords.x/20)+","+std::to_string(coords.y/20);
    EntityFactory entFac;
    if(numKey==1 && playerPlaced){
        //TODO v and while im here add tutorial text
        // std::cout << "AHHHH HH" << std::endl;
        return;
    }
    if(designMap.find(sCoords)==designMap.end()){
        entt::entity e = entFac.createImage(registry, coords, textures.get(static_cast<Textures::ID>(numKey)), false);
        if(numKey == 1){
            playerPlaced = true; 
        }
        designMap.insert({sCoords,numKey});
        designEntityMap.insert({sCoords,e});
    }
}

void Game::removeBlockFromEditor(sf::Vector2f coords){
    std::string sCoords = std::to_string(coords.x/20)+","+std::to_string(coords.y/20);
    EntityFactory entFac;
    if(designMap.find(sCoords)!=designMap.end()){
        if(designMap[sCoords]==1){
            playerPlaced=false;
        }
        designMap.erase(sCoords);
        registry.remove_all(designEntityMap[sCoords]);
        designEntityMap.erase(sCoords);
    }
}

void Game::handleMouseInput(sf::Mouse::Button button, bool isPressed){
    EntityFactory entFac;
    if(button == sf::Mouse::Left && (isPressed) || mouseLClicked){
        sf::Vector2i m = sf::Mouse::getPosition();
        sf::Vector2f rm = mWindow.mapPixelToCoords(m,worldView);
        // std::cout << rm.x << " : " << rm.y << std::endl;
        addBlockToEditor({floor(rm.x/20)*20,floor(rm.y/20)*20});
        // std::cout << "left" << std::endl;
    }else if(button == sf::Mouse::Right && (isPressed) || mouseRClicked){
        // std::cout << "right" << std::endl;
        sf::Vector2i m = sf::Mouse::getPosition();
        sf::Vector2f rm = mWindow.mapPixelToCoords(m,worldView);
        // std::cout << rm.x << " : " << rm.y << std::endl;
        removeBlockFromEditor({floor(rm.x/20)*20,floor(rm.y/20)*20});
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::Up) {
        misMovingUp = isPressed;
    } else if (key == sf::Keyboard::Down) {
        misMovingDown = isPressed;
    } else if (key == sf::Keyboard::Left) {
        misMovingLeft = isPressed;
    } else if (key == sf::Keyboard::Right) {
        misMovingRight = isPressed;
    } else if (key == sf::Keyboard::SemiColon) {
        loadLevel("lvl1");
    } else if (key == sf::Keyboard::L) {
        loadLevel("lvl3");
    }else if (key == sf::Keyboard::Space) {
        if (playerOnFloor) {
            playerJump = 1050;
            playerOnFloor = false;
        }
    }else if(key == sf::Keyboard::Escape){
        loadLevel("menu");
    }else if(key == sf::Keyboard::P){
        LevelHandler lvlH;
        lvlH.saveLevel("lvl3", designMap);  
    }else if(key == sf::Keyboard::X){
        mAttacking = isPressed;
    }else if(key == sf::Keyboard::Enter){
        enterPressed = isPressed;
    }else if(key == sf::Keyboard::Num1){
        numKey = 1;
    }else if(key == sf::Keyboard::Num2){
        numKey = 2;
    }else if(key == sf::Keyboard::Num3){
        numKey = 3;
    }else if(key == sf::Keyboard::Num4){
        numKey = 4;
    }else if(key == sf::Keyboard::Num5){
        numKey = 5;
    }else if(key == sf::Keyboard::Num6){
        numKey = 6;
    }else if(key == sf::Keyboard::Num7){
        numKey = 7;
    }else if(key == sf::Keyboard::Num8){
        numKey = 8;
    }else if(key == sf::Keyboard::Num9){
        numKey = 9;
    }
    // std::cout << numKey << std::endl;
}

void Game::changeToLevelSelect(){
    menuSection = 1;
    EntityFactory entFac;
    menuList.clear();
    registry.clear();
    playerID = entFac.createPlayer(registry,textures, {0,0});
    playerSword = entFac.createPlayerSword(registry,textures, {0,0});

    menuList.push_back(makeMenuButton({250.f,250.f}, 15,2,"LEVEL 1",true));
    menuList.push_back(makeMenuButton({250.f,325.f}, 15,2,"LEVEL 2",false));
    menuList.push_back(makeMenuButton({250.f,400.f}, 15,2,"LEVEL 3",false));
    menuList.push_back(makeMenuButton({250.f,475.f}, 15,2,"CUSTOM LEVEL",false));
    menuList.push_back(makeMenuButton({250.f,550.f}, 15,2,"BACK",false));
}


void Game::updateMenu(sf::Time deltaTime) {
    if (menuClock.getElapsedTime().asSeconds()>0.2){
        maxMenuState = menuList.size();
        // std::cout << "menSize" << maxMenuState << std::endl;
        if(misMovingDown){
            menuState++;
            menuClock.restart();
        }
        if(misMovingUp){
            menuState--;
            menuClock.restart();
        }
        // std::cout << menuState << std::endl;
        if(menuState>=maxMenuState){
            menuState=0;
        }
        if(menuState<0){
            menuState = maxMenuState-1;
        }
    }
    
    for(int i=0; i<menuList.size();i++){
        if(i==menuState){
            registry.get<Text>(menuList[i]).text.setOutlineColor(sf::Color::Red);
        }else{
            registry.get<Text>(menuList[i]).text.setOutlineColor(sf::Color::Black);
        }
    }
    if (enterPressed){
        if(menuSection == 0){
            switch (menuState){
            case 0:
                // currentLevel = "lvl1";
                // loadLevel("lvl1");
                changeToLevelSelect();
                return;
                break;
            case 1:
                // currentLevel = "designer";
                loadLevel("designer");
                initDesigner();
                return;
                break;
            case 2:
                mWindow.close();
                break;
            
            default:
                break;
            }
        }else if(menuSection == 1){
            switch (menuState){
            case 0:
                currentLevel = "lvl1";
                loadLevel("lvl1");
                return;
                break;
            case 1:
                currentLevel = "lvl2";
                loadLevel("lvl2");
                return;
                break;
            case 2:
                currentLevel = "lvl3";
                loadLevel("lvl3");
                return;
                break;
            case 3:
                currentLevel = "lvlCustom";
                loadLevel("lvlCustom");
                return;
                break;
            case 4:
                currentLevel = "menu";
                loadLevel("menu");
                return;
                break;
            
            default:
                break;
            }
        }
    }
}

void Game::initWindow() {
    // mWindow.setView(worldView);
    sf::Vector2u wSize = mWindow.getSize();
    sf::Vector2f vSize = worldView.getSize();
    float sizeX = 1, sizeY = 1;
    float posX = 0, posY = 0;
    float wRat = (float)wSize.x / (float)wSize.y;
    float vRat = vSize.x / vSize.y;
    std::cout << wSize.x << "w" << wSize.y << std::endl;
    std::cout << vSize.x << "v" << vSize.y << std::endl;
    // bool barsTop = true;
    std::cout << "wRat " << wRat << std::endl;
    std::cout << "vRat " << vRat << std::endl;
    if (wRat == vRat) {
    } else if (wRat < vRat) {
        //bars at the topfalsel;
        sizeY = wRat / vRat;
        posY = (1 - sizeY) / 2.f;
    } else {
        std::cout << "barSide" << std::endl;
        sizeX = vRat / wRat;
        posX = (1 - sizeX) / 2.f;
    }
    worldView.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    mWindow.setView(worldView);
}

void Game::initECS() {

}

void Game::run() {
    initWindow();
    initTextures(entList);
    initECS();
    sf::Vector2f ploc = registry.get<Draw>(playerID).sprite.getPosition();
    // std::cout << ploc.x/20 << " : " << ploc.y/20 << std::endl;
    // Entity o(textures.get(Textures::Landscape), -600.f, -200.f, false);
    //todo make this ecs entity instead
    Entity o(textures.get(Textures::Landscape), ploc.x,ploc.y, false);
    o.setOrigin(o.getBoundingBox().width/2,o.getBoundingBox().height/2);
    o.scale(0.5f, 0.5f);
    
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen()) {
        // sf::Time deltaTime = clock.restart();
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }
        // render();
        mWindow.clear(sf::Color::White);
        o.setPostion(worldView.getCenter());
        o.draw(mWindow);
        // y.draw(mWindow);
        // mWindow.draw(t);
        //todo, should always have player just maybe dont always draw them lmao 

        sf::Vector2f plo = registry.get<Draw>(playerID).sprite.getPosition();
        if(plo.y>1500){
            playerDeath();
        }
        // std::cout << plo.x/20 << " : " << plo.y/20 << std::endl;

        if(currentLevel!="menu"){
            worldView.setCenter(registry.get<Draw>(playerID).sprite.getPosition());
        }else{
            worldView.setCenter({400.f,400.f});
        }
        mWindow.setView(worldView);
        
        
        const auto animView = registry.view<Animation, Draw, Movement>();
        for (const entt::entity a : animView) {
            if(animView.get<Animation>(a).animClock.getElapsedTime().asMilliseconds()>animView.get<Animation>(a).idleTime*10){
                animView.get<Animation>(a).animClock.restart();
                std::vector<int> textureList = animView.get<Animation>(a).textureList;
                sf::Sprite sp = animView.get<Draw>(a).sprite;
                sf::Vector2f vel = animView.get<Movement>(a).velocity;
                if(animView.get<Animation>(a).currentTexture==0  && vel.y>=0 && vel.x!=0){
                    sp.setTexture(textures.get(static_cast<Textures::ID>(textureList[1])));
                    animView.get<Animation>(a).currentTexture=1;
                }else if(animView.get<Animation>(a).currentTexture==1 && vel.y>=0 && vel.x!=0){
                    sp.setTexture(textures.get(static_cast<Textures::ID>(textureList[0])));
                    animView.get<Animation>(a).currentTexture=0;
                }else if(animView.get<Animation>(a).currentTexture==2 && animView.get<Animation>(a).attackClock.getElapsedTime().asSeconds()>0.5){
                    animView.get<Animation>(a).canAttack = true;
                    registry.get<Animation>(playerID).attackClock.restart();
                    sp.setTexture(textures.get(static_cast<Textures::ID>(textureList[0])));
                    animView.get<Animation>(a).currentTexture=0;
                    if(a == playerID){
                        registry.get<Draw>(playerSword).visible=false;
                    }
                }
            animView.get<Draw>(a).sprite = sp;
            }
        }

        

        const auto view = registry.view<Draw>();
        sf::Vector2f pPos = view.get<Draw>(playerID).sprite.getPosition();
        if(view.get<Draw>(playerSword).facing){
            view.get<Draw>(playerSword).sprite.setPosition(pPos.x+26,pPos.y+8);
        }else
        {
            view.get<Draw>(playerSword).sprite.setPosition(pPos.x-23,pPos.y+8);
        }
        
        // sSprite.setPosition();
        // registry.get<Draw>(playerSword).sprite = sSprite;
        
       
        
        
        for (const entt::entity e : view) {
            if(!(e==playerID && currentLevel.substr(0,3)!="lvl")){
                if(view.get<Draw>(e).visible){
            
                sf::Sprite spri = view.get<Draw>(e).sprite;
                mWindow.draw(spri);
                }
            }else{
                // mWindow.draw(registry.get<Draw>(logo).sprite);
            }
        }
        const auto view2 = registry.view<Text>();
        if(!view2.empty()){
            for (const entt::entity e : view2) {
                sf::Text text = view2.get<Text>(e).text;
                mWindow.draw(text);
            }
        }

        const auto view3 = registry.view<DrawShape>();
        if(!view2.empty()){
            for (const entt::entity e : view3) {
                sf::RectangleShape re = view3.get<DrawShape>(e).rect;
                mWindow.draw(re);
            }
        }

        // for(auto x : imageList){
        //     mWindow.draw(registry.get<Draw>(x).sprite);
        // }

        

        mWindow.display();
    }
}

entt::entity Game::makeMenuButton(sf::Vector2f pos, int length, int height, std::string text, bool primary){
    EntityFactory entFac;
    for(int i=0;i<=height;i++){ 
        for(int x=0;x<=length;x++){
            entFac.createPlatform(registry, textures, {pos.x+(x*20),pos.y+(i*20)});
        }
    }
    entt::entity e = entFac.createText(registry,fonts, {pos.x+(length*20)/2,pos.y+(height*20)/2}, text, 40);
    if(primary){
        registry.get<Text>(e).text.setOutlineColor(sf::Color::Red);
    }
    return e;
    
}

void Game::initDesigner(){
    sf::Vector2f pos = worldView.getCenter();;
    //
    // pos.y += (worldView.getSize().y/;
    EntityFactory entFac;

   
    imageList.clear();
    rectList.clear();
    for(int i=1;i<=9;i++){
        entt::entity e = entFac.createImage(registry, {pos.x+75*i,pos.y},textures.get(static_cast<Textures::ID>(i)), false);
        imageList.push_back(e);
        entt::entity t = entFac.createText(registry, fonts, {(pos.x+75*i)-6,pos.y+35},std::to_string(i),25);
        textList.push_back(t);
        entt::entity r = entFac.createRectangle(registry, {(pos.x+75*i)-15,pos.y-10}, {65.f,70.f}, sf::Color::Red, false);
        rectList.push_back(r);
    }
}

void Game::updateDesignerHUD(){
    entt::basic_view recView = registry.view<DrawShape>();
    sf::Vector2f pos = worldView.getCenter();
    pos.x -= worldView.getSize().x/2.1;
    pos.y -= worldView.getSize().y/2.1;
    for(int i=0;i<9;i++){
        registry.get<Draw>(imageList[i]).sprite.setPosition({pos.x+75*i,pos.y});
        registry.get<Text>(textList[i]).text.setPosition({(pos.x+75*i)-6,pos.y+35});
        registry.get<DrawShape>(rectList[i]).rect.setPosition({(pos.x+75*i)-15,pos.y-10});
    }
}

void Game::updateDesigner(sf::Time deltaTime){//memory leak isn't coming from this function at least
    entt::basic_view recView = registry.view<DrawShape>();
    for(int i=0;i<9;i++){
        sf::RectangleShape r = recView.get<DrawShape>(rectList[i]).rect;
        if((i+1)==numKey){
            r.setOutlineColor(sf::Color::Red);
            r.setOutlineThickness(3);
        }else{
            r.setOutlineColor(sf::Color::Black);
            r.setOutlineThickness(1);
        }
        recView.get<DrawShape>(rectList[i]).rect = r;
    }
    int viewVelocity = 200;
    sf::Vector2f viewV = {0.f,0.f};
    
    if(misMovingUp){
        viewV.y -= viewVelocity;
    }
    if(misMovingDown){
        viewV.y += viewVelocity;
    }
    if(misMovingLeft){
        viewV.x -= viewVelocity;
    }
    if(misMovingRight){
        viewV.x += viewVelocity;
    }
    worldView = mWindow.getView();
    worldView.move(viewV*deltaTime.asSeconds());
    mWindow.setView(worldView);
    updateDesignerHUD();

}

void Game::updateLevel(sf::Time deltaTime) {
     if (playerOnRoof) {
            playerJump = -50;
            if (playerOnFloor) {
                playerJump = -1;
            }
        }
        if (playerJump > -400) {
            playerJump -= 40;
        } else {
            playerJump = -0;
        }
        float playerGrav = 270.f;
        sf::Vector2f movement(0.f, (-1 * playerJump) + playerGrav);
        playerOnFloor = false;
        playerOnRoof = false;

        if (misMovingLeft) {
            movement.x -= 370.f;
            entt::entity plandsw [2] = {playerID,playerSword};
            for(auto e : plandsw){
                if (registry.get<Draw>(e).facing) {
                    sf::Sprite sp = registry.get<Draw>(e).sprite;
                    sf::IntRect ir = sp.getTextureRect();
                    sp.setTextureRect(sf::IntRect(ir.width, 0, -ir.width, ir.height));
                    registry.get<Draw>(e).sprite = sp;
                    registry.get<Draw>(e).facing = false;
                }
            }
        }
        if (misMovingRight) {
            movement.x += 370.f;
             entt::entity plandsw [2] = {playerID,playerSword};
                for(auto e : plandsw){
                if (!registry.get<Draw>(e).facing) {
                    sf::Sprite sp = registry.get<Draw>(e).sprite;
                    sf::IntRect ir = sp.getTextureRect();
                    sp.setTextureRect(sf::IntRect(0, 0, -ir.width, ir.height));
                    registry.get<Draw>(e).sprite = sp;
                    registry.get<Draw>(e).facing = true;
                }
            }
        }

        if (mAttacking && registry.get<Animation>(playerID).canAttack && registry.get<Animation>(playerID).attackClock.getElapsedTime().asSeconds()>0.55){
            registry.get<Animation>(playerID).canAttack = false;
            registry.get<Animation>(playerID).attackClock.restart();
            std::vector<int> textureList = registry.get<Animation>(playerID).textureList;
            sf::Sprite sp = registry.get<Draw>(playerID).sprite;
            sp.setTexture(textures.get(static_cast<Textures::ID>(textureList[2])));
            registry.get<Draw>(playerID).sprite = sp;
            registry.get<Animation>(playerID).currentTexture = 2;
            registry.get<Draw>(playerSword).visible=true;
        }

        const auto aiView = registry.view<Movement, AI, Draw>();
        for (const entt::entity e : aiView) {

            sf::Vector2f currentPos = aiView.get<Draw>(e).sprite.getPosition();
            // std::cout << "pos " << currentPos.x << std::endl;
            std::vector<sf::Vector2f> path  = aiView.get<AI>(e).path;
            // std::cout << "Pth: " << path.size() <<  std::endl;
            if(path.size() > 0){
                sf::Sprite sp = aiView.get<Draw>(e).sprite;
                sf::IntRect ir = sp.getTextureRect();
                if(currentPos.x > path[1].x){
                    aiView.get<Movement>(e).velocity.x = -enemyMoveSpeed;
                    if(aiView.get<AI>(e).facing){
                        aiView.get<AI>(e).facing = false;
                        
                        sp.setTextureRect(sf::IntRect(ir.width, 0, -ir.width, ir.height));
                    }
                }else if(currentPos.x < path[0].x){
                    
                    aiView.get<Movement>(e).velocity.x = enemyMoveSpeed;
                    if(!aiView.get<AI>(e).facing){
                        aiView.get<AI>(e).facing = true;
                        sp.setTextureRect(sf::IntRect(0, 0, -ir.width, ir.height));
                    }
                }else if(aiView.get<Movement>(e).velocity.x == 0){
                    if(!aiView.get<AI>(e).facing){
                        aiView.get<AI>(e).facing = true;
                        sp.setTextureRect(sf::IntRect(0, 0, -ir.width, ir.height));
                    }
                    aiView.get<Movement>(e).velocity.x = enemyMoveSpeed;
                    
                }
                aiView.get<Draw>(e).sprite = sp;
            }
        }

        const auto shootView = registry.view<AttackRanged, Animation, Draw>();
        EntityFactory eF;
        for (const entt::entity sh : shootView) {
            if(shootView.get<Animation>(sh).attackClock.getElapsedTime().asSeconds()>3 && sh!=playerID){
                shootView.get<Animation>(sh).attackClock.restart();
                sf::Vector2f pos = shootView.get<Draw>(sh).sprite.getPosition();
                pos.y += 17;
                eF.createBullet(registry,textures,pos, -150);

            }
        }

        const auto bulletView = registry.view<Draw>();
        for (const entt::entity bu : bulletView) {
            if (bulletView.get<Draw>(bu).label.compare("bullet")==0){
                // std::cout << "BU " << bulletView.get<Draw>(bu).aliveTime.getElapsedTime().asSeconds() << std::endl;
                if(bulletView.get<Draw>(bu).aliveTime.getElapsedTime().asSeconds()>3){
                    registry.remove_all(bu);
                }
                
            }
        }

        

        const auto killView = registry.view<Draw, Physics>();
        if(killView.get<Draw>(playerSword).visible){
            for (const entt::entity j : killView) {
                sf::FloatRect jBounds = killView.get<Draw>(j).sprite.getGlobalBounds();
                sf::FloatRect swordBounds = killView.get<Draw>(playerSword).sprite.getGlobalBounds();
                if(jBounds.intersects(swordBounds) && (killView.get<Draw>(j).label=="enemy" || killView.get<Draw>(j).label=="bullet")){
                    // view2.get<Draw>(j).visible = false;
                    registry.remove_all(j);
                }else if(jBounds.intersects(swordBounds) && killView.get<Draw>(j).label=="boss"){
                    bossHealth--;
                    std::cout << bossHealth << std::endl;
                    if(bossHealth<=0){
                        playerWin();
                    }
                }
            }
        }
            
        registry.get<Movement>(playerID).velocity = movement;
        const auto view = registry.view<Draw, Movement>();
        const auto view2 = registry.view<Draw, Physics>();
        // std::cout << "V1: " << view.size() << std::endl;
        // std::cout << "V2: " << view2.size() << std::endl;
        for (const entt::entity e : view) {
            float xIdealCoordinate = 0;
            float yIdealCoordinate = 0;
            bool movex = true;
            bool movey = true;
            sf::Sprite spriX = view.get<Draw>(e).sprite;
            sf::Sprite spriY = view.get<Draw>(e).sprite;
            
            sf::Vector2f vel = view.get<Movement>(e).velocity * deltaTime.asSeconds();
            sf::Vector2f velx = {vel.x, 0.f};
            sf::Vector2f vely = {0.f, vel.y};
            spriX.move(velx);
            spriY.move(vely);
            // spri.move(velx);

            sf::FloatRect eXBounds = spriX.getGlobalBounds();
            sf::FloatRect eYBounds = spriY.getGlobalBounds();
            // if(collisionClock.getElapsedTime().asSeconds()>0.1){
            //     collisionClock.restart();
            if(e == playerID){
                for (const entt::entity j : view2) {
                    std::string jLabel = view2.get<Draw>(j).label;
                    sf::FloatRect jBounds = view2.get<Draw>(j).sprite.getGlobalBounds();
                    // //TODO make it so player dies when touching enemy


                    if (j != e &&  view2.get<Physics>(j).hasCollision) {
                        if(jLabel == "bullet" || jLabel== "enemy" || jLabel == "boss"){
                            if(jBounds.intersects(view.get<Draw>(e).sprite.getGlobalBounds())){
                                playerDeath();
                                break;
                            }
                        }else {
                            if (jBounds.intersects(eXBounds)) {
                                // if(jLabel == "bullet" || jLabel== "enemy" ){
                                //     playerDeath();
                                //     break;
                                // }else{
                                if (!(eXBounds.left <= jBounds.left + jBounds.width && !(jBounds.left <= eXBounds.left))) {
                                    // std::cout << "IntRight" << std::endl;
                                    xIdealCoordinate = jBounds.left + jBounds.width;
                                } else {
                                    // std::cout << "IntLeft" << std::endl;
                                    xIdealCoordinate = jBounds.left - eXBounds.width;
                                }
                                velx = {0.f, 0.f};
                            }
                            if (jBounds.intersects(eYBounds)) {
                                // if(jLabel == "bullet" || jLabel== "enemy" ){
                                //     playerDeath();
                                //     break;
                                // }else{
                                if (!(eYBounds.top <= jBounds.top + jBounds.height && !(jBounds.top <= eYBounds.top))) {
                                    yIdealCoordinate = jBounds.top + jBounds.height;
                                } else {
                                    yIdealCoordinate = jBounds.top - eYBounds.height;
                                    if (e == playerID) {
                                        playerOnFloor = true;
                                    }
                                }
                                vely = {0.f, 0.f};
                            }
                        }
                    }
                }
            }
            sf::Sprite sprit = view.get<Draw>(e).sprite;
            sprit.move(velx + vely);
            if (yIdealCoordinate != 0) {
                sprit.setPosition(sprit.getPosition().x, yIdealCoordinate);
            }
            if (xIdealCoordinate != 0) {
                sprit.setPosition(xIdealCoordinate, sprit.getPosition().y);
            }
            view.get<Draw>(e).sprite = sprit;
        
    }     
}

void Game::update(sf::Time deltaTime) {
    if(currentLevel.compare("menu")==0){    
       updateMenu(deltaTime);
    }else if(currentLevel.compare("designer")==0){
        updateDesigner(deltaTime);
    }else{
        updateLevel(deltaTime);
    }
}

void Game::playerDeath(){
    EntityFactory ef;
    entt::entity e = ef.createText(registry,fonts,registry.get<Draw>(playerID).sprite.getPosition(), "YOU DIED", 60);
    mWindow.draw(registry.get<Text>(e).text);
    registry.get<Physics>(playerID).hasCollision = false;
    // if(!deathLoading){
        // deathLoading = true;
        loadLevel(currentLevel);
}

void Game::playerWin(){
    EntityFactory ef;
    entt::entity e = ef.createText(registry,fonts,registry.get<Draw>(playerID).sprite.getPosition(), "LEVEL COMPLETE", 60);
    mWindow.draw(registry.get<Text>(e).text);
    registry.get<Physics>(playerID).hasCollision = false;

        loadLevel("menu");
}

