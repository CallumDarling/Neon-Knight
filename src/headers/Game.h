#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cassert>
#include "ResourceHandler.h"
#include "Entity.h"

namespace Textures{
     enum ID { Landscape, Player, Henchman, Boss, Block};
}

class Game{
    public:
            Game();
        void run();
        bool misMovingUp = false;
        bool misMovingDown = false;
        bool misMovingLeft = false;
        bool misMovingRight = false;
         
    private:
        void processEvents();
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
        
        ResourceHandler<sf::Texture, Textures::ID> textures;
        const sf::Time TimePerFrame = sf::seconds(1.f/60.f);

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

