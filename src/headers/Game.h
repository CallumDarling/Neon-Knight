#ifndef GAME
#define GAME

// #include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "LevelHandler.h"
#include "Entity.h"
#include "EntityFactory.h"



class Game{
    public:
            Game();
        void run();
        bool misMovingUp = false;
        bool misMovingDown = false;
        bool misMovingLeft = false;
        bool misMovingRight = false;

         
    private:
        void initECS();
        void processEvents();
        void initWindow();
        bool initTextures(std::vector<Entity>& entList);
        void update(sf::Time deltaTime);
        void render();
        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
        


    private:
        sf::RenderWindow mWindow;
        sf::View worldView;
        sf::Texture mTexture;
        sf::Sprite mPlayer;
        sf::Sprite mBoss;
        sf::Sprite mHench;
        std::vector<Entity> entList;
        float playerJump = 0;
        bool playerOnFloor = false;
        bool playerOnRoof = false;
        entt::registry registry;
        ResourceHandler<sf::Texture, Textures::ID> textures;
        const sf::Time TimePerFrame = sf::seconds(1.f/60.f);
        entt::entity playerID;

};

#endif

    // mTexture(),
    // mPlayer(){
    //     if (mTexture.loadFromFile("att2.png")){
    //         //lol
    //     }
    //     mPlayer.setTexture(mTexture);
    //     mPlayer.setPosition(100.f, 100.f); 
    //     mPlayer.scale(sf::Vector2f(3,3));
    // }

