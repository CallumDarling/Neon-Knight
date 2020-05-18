#ifndef GAME
#define GAME

// #include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "LevelHandler.h"
#include "Entity.h"
#include "EntityFactory.h"
#include <math.h>
#include <map>



class Game{
    public:
            Game();
        void run();
        bool misMovingUp = false;
        bool misMovingDown = false;
        bool misMovingLeft = false;
        bool misMovingRight = false;
        bool enterPressed = false;

         
    private:
        void initECS();
        void processEvents();
        void initWindow();
        bool initTextures(std::vector<Entity>& entList);
        void update(sf::Time deltaTime);
        void updateLevel(sf::Time deltaTime);
        void updateMenu(sf::Time deltaTime);
        void initDesigner();
        void updateDesigner(sf::Time deltaTime);
        void render();
        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
        void loadLevel(std::string level);
        void initMenu();
        void handleMouseInput(sf::Mouse::Button button, bool isPressed);
        void addBlockToEditor(sf::Vector2f coords);
        void removeBlockFromEditor(sf::Vector2f coords);
        void updateDesignerHUD();
        entt::entity makeMenuButton(sf::Vector2f pos, int length, int height, std::string text, bool primary);
        // bool saveLevelToFile(std::string filename);
        


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
        ResourceHandler<sf::Font, Fonts::ID> fonts;
        const sf::Time TimePerFrame = sf::seconds(1.f/60.f);
        entt::entity playerID;
        std::string currentLevel =  "menu";
        std::vector<entt::entity> menuList;
        int maxMenuState;
        int menuState = 0;
        sf::Clock menuClock;
        entt::entity logo;
        std::vector<entt::entity> imageList;
        std::vector<entt::entity> rectList;
        std::vector<entt::entity> textList;
        int numKey = 1;
        bool mouseLClicked = false;
        bool mouseRClicked = false;
        std::map<std::string, int> designMap;
        std::map<std::string, entt::entity> designEntityMap;
        bool playerPlaced;

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

