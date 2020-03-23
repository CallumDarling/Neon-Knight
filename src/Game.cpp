#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include "headers/ResourceHandler.h"
#include "headers/Game.h"

Game::Game(): 
    mWindow(sf::VideoMode(1600,900), "SFML Application"){
        mWindow.setKeyRepeatEnabled(false);
    }

void Game::run(){
    // std::cout << "Running" << std::endl;
    textures.load(Textures::Player, "media/textures/att2.png");
    textures.load(Textures::Henchman, "media/textures/hench1.png");
    textures.load(Textures::Boss, "media/textures/boss1.png");
    mPlayer.setTexture(textures.get(Textures::Player));
    mPlayer.scale(sf::Vector2f(3.f,3.f));
    mHench.setTexture(textures.get(Textures::Henchman));
    mHench.scale(sf::Vector2f(3.f,3.f));
    mBoss.setTexture(textures.get(Textures::Boss));
    mBoss.scale(sf::Vector2f(3.f,3.f));
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
        render();
    }
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
    mPlayer.move(movement * deltaTime.asSeconds());
    mHench.move(movement);
}

void Game::render(){
    mWindow.clear();
    mWindow.draw(mPlayer);
    mWindow.draw(mHench);
    mWindow.draw(mBoss);
    mWindow.display();
}