#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cassert>

namespace Textures{
    enum ID { Landscape, Plane, Missile};
}
template <typename Resource, typename Identifier>
class ResourceHolder{
    public:
        void load(Identifier id, const std::string& filename);
        sf::Resource& get(Identifier id);
        const sf::Resource& get(Identifier id) const;
    private:
        std::map<Identifier,
        std::unique_ptr<sf::Resource>> mResourceMap;
};

    void ResourceHolder::load(Identifier id, const std::string& filename){
        std::unique_ptr<sf::Resource> Resource(new sf::Resource());
        if(!Resource->loadFromFile(filename)){
            throw std::runtime_error("ResourceHolder::load - Failed to load" + filename);
        }
        auto inserted = mResourceMap.insert(std::make_pair(id, std::move(Resource)));
        assert(inserted.second);
    }
    

    sf::Resource& ResourceHolder::get(Identifier id){
        auto found = mResourceMap.find(id);
        assert(found != mResourceMap.end());
        return *found->second;
    }
    const sf::Resource& ResourceHolder::get(Identifier id) const{
        auto found = mResourceMap.find(id);
        return *found->second;
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
        ResourceHolder textures;
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

Game::Game(): 
    mWindow(sf::VideoMode(1600,900), "SFML Application"),
    textures(){
        textures.load(Textures::Plane, "att2.png");
    }


void Game::run(){
    mPlayer.setTexture(textures.get(Textures::Plane));
    mPlayer.scale(sf::Vector2f(3.f,3.f));
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
}

void Game::render(){
    mWindow.clear();
    mWindow.draw(mPlayer);
    mWindow.display();
}

int main(){
   Game game;
   game.run();
}