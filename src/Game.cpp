#include "headers/Game.h"

Game::Game() : worldView(sf::FloatRect(32.f, 32.f, 768.f, 432.f)),
               mWindow(sf::VideoMode(1600, 900), "SFML Application", sf::Style::Fullscreen) {
    mWindow.setKeyRepeatEnabled(true);
    mWindow.setVerticalSyncEnabled(true);
    // mWindow.setFramerateLimit(60);
}

void Game::loadLevel(std::string level){
    registry.clear();
    EntityFactory eFac;
    LevelHandler lvlH;
    if(level.compare("lvl2")!=0){
        playerID = eFac.createPlayer(registry, textures);
    }
    lvlH.loadLevel("levels/"+level, textures, entList, registry);
}

bool Game::initTextures(std::vector<Entity> &eList) {
    try {
        textures.load(Textures::Player, "media/textures/att2.png");
        textures.load(Textures::Henchman, "media/textures/hench1.png");
        textures.load(Textures::Boss, "media/textures/boss1.png");
        textures.load(Textures::Block, "media/textures/basicblock.png");
        textures.load(Textures::Landscape, "media/textures/bg.jpg");
        textures.load(Textures::HealthBar, "media/textures/healthbar.png");
        textures.load(Textures::Ladder, "media/textures/ladder.png");
        textures.load(Textures::Platform, "media/textures/platform.png");
        textures.load(Textures::Door, "media/textures/door.png");
        textures.load(Textures::Bullet, "media/textures/bullet.png");
    } catch (const std::exception &e) {
        return 0;
    }
    loadLevel("lvl2");
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
        case sf::Event::Closed:
            mWindow.close();
            break;
        }
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
        Entity player = entList[0];
        entList.clear();
        entList.push_back(player);
        LevelHandler lvlH;
        lvlH.loadLevel("levels/lvl1", textures, entList, registry);
    } else if (key == sf::Keyboard::Space) {
        if (playerOnFloor) {
            playerJump = 1050;
            playerOnFloor = false;
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
    Entity o(textures.get(Textures::Landscape), -600.f, -200.f, false);
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
        mWindow.clear();
        o.draw(mWindow);
        //todo, should always have player just maybe dont always draw them lmao 
        if(registry.has<Draw>(playerID)){
            worldView.setCenter(registry.get<Draw>(playerID).sprite.getPosition());
        }else{
            worldView.setCenter({0.f,0.f});
        }
        mWindow.setView(worldView);
        const auto view = registry.view<Draw>();
        for (const entt::entity e : view) {
            sf::Sprite spri = view.get<Draw>(e).sprite;
            mWindow.draw(spri);
        }
        mWindow.display();
    }
}

void Game::update(sf::Time deltaTime) {
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

    if (misMovingUp) {
        // movement.y -= 470.f;
    }
    if (misMovingDown) {
        // movement.y += 200.f;
    }
    if (misMovingLeft) {
        movement.x -= 370.f;
        if (registry.get<Draw>(playerID).facing) {
            sf::Sprite sp = registry.get<Draw>(playerID).sprite;
            sf::IntRect ir = sp.getTextureRect();
            sp.setTextureRect(sf::IntRect(ir.width, 0, -ir.width, ir.height));
            registry.get<Draw>(playerID).sprite = sp;
            registry.get<Draw>(playerID).facing = false;
        }
    }
    if (misMovingRight) {
        movement.x += 370.f;
        if (!registry.get<Draw>(playerID).facing) {
            sf::Sprite sp = registry.get<Draw>(playerID).sprite;
            sf::IntRect ir = sp.getTextureRect();
            sp.setTextureRect(sf::IntRect(0, 0, -ir.width, ir.height));
            registry.get<Draw>(playerID).sprite = sp;
            registry.get<Draw>(playerID).facing = true;
        }
    }
    const auto aiView = registry.view<Movement, AI, Draw>();
    for (const entt::entity e : aiView) {
        sf::Vector2f currentPos = aiView.get<Draw>(e).sprite.getPosition();
        std::cout << "pos " << currentPos.x << std::endl;
        std::vector<sf::Vector2f> path  = aiView.get<AI>(e).path;
        // std::cout << "Pth: " << path.size() <<  std::endl;
        if(path.size() > 0){
            std::cout << "n2 " << path[1].x << std::endl;
            if(currentPos.x > path[1].x){
                aiView.get<AI>(e).facing = false;
                aiView.get<Movement>(e).velocity.x = -enemyMoveSpeed;
            }else if(currentPos.x < path[0].x){
                aiView.get<AI>(e).facing = true;
                aiView.get<Movement>(e).velocity.x = enemyMoveSpeed;
            }else if(aiView.get<Movement>(e).velocity.x == 0){
                aiView.get<AI>(e).facing = true;
                aiView.get<Movement>(e).velocity.x = enemyMoveSpeed;
            }
        }
    }



    const auto gravView = registry.view<Movement, Physics>();
    for (const entt::entity e : gravView) {
        if (gravView.get<Physics>(e).hasGrav) {
            if (gravView.get<Movement>(e).velocity.y <= gravity);
            gravView.get<Movement>(e).velocity.y = gravity;
        }
    }

    registry.get<Movement>(playerID).velocity = movement;
    const auto view = registry.view<Draw, Movement>();
    const auto view2 = registry.view<Draw, Physics>();
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

        for (const entt::entity j : view2) {
            sf::FloatRect jBounds = view2.get<Draw>(j).sprite.getGlobalBounds();
            // //TODO find better way of checking if they are not the same object
            if (j != e) {
                if (jBounds.intersects(eXBounds)) {
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
