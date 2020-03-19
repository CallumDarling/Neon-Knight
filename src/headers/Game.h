#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cassert>
#include "ResourceHandler.h"

namespace Textures{
     enum ID { Landscape, Plane, Missile};
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
        void update(sf::Time deltaTime);
        void render();
        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

    private:
        sf::RenderWindow mWindow;
        sf::Texture mTexture;
        sf::Sprite mPlayer;
        ResourceHandler<sf::Texture, Textures::ID> textures;
        const sf::Time TimePerFrame = sf::seconds(1.f/60.f);

};


    // mTexture(),
    // mPlayer(){
    //     if (mTexture.loadFromFile("att2.png")){
    //         //lol
    //     }
    //     mPlayer.setTexture(mTexture);
    //     mPlayer.setPosition(100.f, 100.f); 
    //     mPlayer.scale(sf::Vector2f(3,3));

    // }

