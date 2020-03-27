#include "headers/Game.h"




Game::Game(): 
    worldView(sf::FloatRect(32.f, 32.f, 768.f, 432.f)),
    mWindow(sf::VideoMode(1600,900), "SFML Application", sf::Style::Fullscreen){
        mWindow.setKeyRepeatEnabled(false);
        mWindow.setVerticalSyncEnabled(true);
        // mWindow.setFramerateLimit(60);
    }

bool Game::initTextures(std::vector<Entity>& eList){
    try{
        textures.load(Textures::Player, "media/textures/att2.png");
        textures.load(Textures::Henchman, "media/textures/hench1.png");
        textures.load(Textures::Boss, "media/textures/boss1.png");
        textures.load(Textures::Block, "media/textures/basicblock.png");
        textures.load(Textures::Landscape, "media/textures/bg.jpg");
    }catch(const std::exception& e){
        return 0;
    }
    Entity e(textures.get(Textures::Player), 200.f, 200.f, true);
    eList.push_back(e);
    Entity ee(textures.get(Textures::Henchman), 250.f, 0.f, true);
    eList.push_back(ee);
    Entity eee(textures.get(Textures::Boss), 200.f, 400.f, true);
    eList.push_back(eee);
    for(int i=0; i<20; i++){
        Entity eeee(textures.get(Textures::Block), i*20.f, 600.f, false);
        eList.push_back(eeee);
    }

    return 1;
}

void Game::processEvents(){
    sf::Event event;
    while(mWindow.pollEvent(event)){
        switch(event.type){
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case sf::Event::Closed:
                mWindow.close();
                break;

        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed){
    if( key == sf::Keyboard::Up){
        misMovingUp = isPressed;
    }else if( key == sf::Keyboard::Down){
        misMovingDown = isPressed;
    }else if( key == sf::Keyboard::Left){
        misMovingLeft = isPressed;
    }else if( key == sf::Keyboard::Right){
        misMovingRight = isPressed;
    }else if( key == sf::Keyboard::SemiColon){
        Entity player = entList[0];
        entList.clear();
        entList.push_back(player);
        LevelHandler lvlH;
        lvlH.loadLevel("levels/lvl1", textures , entList);
    }else if( key == sf::Keyboard::Space){
        if(playerOnFloor){
            playerJump = 700;
            playerOnFloor = false;
        }
    }

}

void Game::initWindow(){
    // mWindow.setView(worldView);
    sf::Vector2u wSize  = mWindow.getSize();
    sf::Vector2f vSize = worldView.getSize();
    float sizeX = 1, sizeY = 1;
    float posX =0, posY = 0;
    float wRat = (float) wSize.x / (float) wSize.y;
    float vRat = vSize.x / vSize.y;
    
    std::cout << wSize.x << "w" << wSize.y << std::endl;
    std::cout << vSize.x << "v" << vSize.y << std::endl;
    // bool barsTop = true;
    std::cout << "wRat " << wRat << std::endl;
    std::cout << "vRat " << vRat << std::endl;
    if(wRat==vRat){
    }else if(wRat < vRat){
        //bars at the top
        std::cout << "barTop" << std::endl;
        sizeY = wRat / vRat;
        posY = (1-sizeY) / 2.f;

    }else{
        std::cout << "barSide" << std::endl;
        sizeX = vRat / wRat;
        posX = (1-sizeX) / 2.f; 
    }
    worldView.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    // worldView.setCenter(entList[0].getPosition());
    mWindow.setView(worldView);


    // std::cout << "wS: "<<size.x << "x" << size.y << std::endl;

}


void Game::run(){
    initWindow();
    initTextures(entList);

    Entity o(textures.get(Textures::Landscape), -600.f, -200.f, false);
    o.scale(0.5f, 0.5f);


    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(mWindow.isOpen()){
        // sf::Time deltaTime = clock.restart();
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate > TimePerFrame){
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }

        // render();
        mWindow.clear();
        o.draw(mWindow);
        // mWindow.draw(mPlayer);
        worldView.setCenter(entList[0].getPosition());
        mWindow.setView(worldView);

        for(std::size_t i=0; i<entList.size(); ++i) {
            
            entList[i].draw(mWindow);
        }
        mWindow.display();
    }
}


void Game::update(sf::Time deltaTime){

    sf::Vector2f movement(0.f, (-1*playerJump)+270.f);
    if(playerJump>-100){
        playerJump-=20;
    }else{
        playerJump=0;
    }

    if(misMovingUp){
        // movement.y -= 470.f;
    }if(misMovingDown){
        // movement.y += 200.f;
    }if(misMovingLeft){
        movement.x -= 300.f;
    }if(misMovingRight){
        movement.x += 300.f;
    }  

    for(std::size_t i=0; i<entList.size(); ++i) {
        std::vector<Entity> intersects;
        bool c = true;
        if(entList[i].getGrav()){
            entList[i].setVelocity(0.f,100.f * deltaTime.asSeconds());
        }
        
        entList[0].setVelocity(movement * deltaTime.asSeconds());
        entList[i].move();
        for(std::size_t j=0; j<entList.size(); ++j) {
            if(entList[i].getBoundingBox().intersects(entList[j].getBoundingBox()) && i!=j){
                // std::cout << "intersects:" << i << " " << j << std::endl;
                c = false;
                intersects.push_back(entList[j]);
            } 
        }
        if(!c){
            bool v = true;
            sf::Vector2f vol = entList[i].getVelocity();
            entList[i].setVelocity(vol.x*-1.f,0);
            entList[i].move();
            for(std::size_t j=0; j<intersects.size(); ++j) {
                if(entList[i].getBoundingBox().intersects(intersects[j].getBoundingBox())){
                    v = false;
                    if(i==0){
                            playerOnFloor = true;
                       }
                    
                }
            }
            if(!v){
                bool b= true;
                entList[i].setVelocity(vol.x,vol.y*-1.f);
                entList[i].move();
                for(std::size_t j=0; j<intersects.size(); ++j) {
                    if(entList[i].getBoundingBox().intersects(intersects[j].getBoundingBox())){
                       b = false;
                    }
                }
                if(!b){
                    entList[i].setVelocity(vol.x*-1.f,0);
                    entList[i].move();
                }
            }
        }
        
    }
}
