#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include "headers/ResourceHandler.h"
#include "headers/Game.h"



Game::Game(): 
    worldView(sf::FloatRect(32.f, 32.f, 1600.f, 900.f)),
    mWindow(sf::VideoMode(1600,900), "SFML Application"){
        mWindow.setKeyRepeatEnabled(false);
        
    }

bool Game::initTextures(std::vector<Entity>& eList){
    try{
        textures.load(Textures::Player, "media/textures/att2.png");
        textures.load(Textures::Henchman, "media/textures/hench1.png");
        textures.load(Textures::Boss, "media/textures/boss1.png");
    }catch(const std::exception& e){
        return 0;
    }
    Entity e(textures.get(Textures::Player), 200.f, 200.f);
    eList.push_back(e);
    Entity ee(textures.get(Textures::Henchman), 200.f, 0.f);
    eList.push_back(ee);
    Entity eee(textures.get(Textures::Boss), 200.f, 400.f);
    eList.push_back(eee);
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
    }
}


void Game::run(){
    
    mWindow.setView(worldView);
    initTextures(entList);
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
        mWindow.draw(mPlayer);
        worldView.setCenter(entList[0].getPosition());
        mWindow.setView(worldView);

        for(std::size_t i=0; i<entList.size(); ++i) {
            
            entList[i].draw(mWindow);
        }
        mWindow.display();
    }
}


void Game::update(sf::Time deltaTime){

    sf::Vector2f movement(0.f, 0.f);

    if(misMovingUp){
        movement.y -= 350.f;
    }if(misMovingDown){
        movement.y += 350.f;
    }if(misMovingLeft){
        movement.x -= 350.f;
    }if(misMovingRight){
        movement.x += 350.f;
    }  
    
    for(std::size_t i=0; i<entList.size(); ++i) {
        bool c = true;
        entList[i].setVelocity(0.f,100.f * deltaTime.asSeconds());
        entList[0].setVelocity(movement * deltaTime.asSeconds());
        for(std::size_t j=0; j<entList.size(); ++j) {
            if(entList[i].getBoundingBox().intersects(entList[j].getBoundingBox()) && i!=j){
                std::cout << "intersects:" << i << " " << j << std::endl;
                c = false;
                
            }
            
        }
        if(c){
            entList[i].move();
        }
        
    }
}
