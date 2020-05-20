#include "headers/Game.h"
// simply intialising the window and world view
Game::Game() : worldView(sf::FloatRect(32.f, 32.f, 768.f, 432.f)),
               mWindow(sf::VideoMode(1600, 900), "Neon Knight", sf::Style::Fullscreen) {
    mWindow.setKeyRepeatEnabled(true);
    mWindow.setVerticalSyncEnabled(true);
    mWindow.setFramerateLimit(60);
}

// function to load levels, it calls the loadLevel function in LevelHandler but also adds any aditional features of levels that are not stored in the files
// such as tutorial text or menu buttons (menu is a pseudo level)
void Game::loadLevel(std::string level) {
    //resets variables and clears the registry so that the new entities can be added
    kill = false;
    win = false;
    registry.clear();
    bossHealth = 15;
    EntityFactory eFac;
    LevelHandler lvlH;
    // if the level being loaded is the menu level then add the menu buttons
    if (level.compare("menu") == 0) {
        // std::cout << "compare menu" << std::endl;
        // playerID = eFac.createPlayer(registry, textures,{400.f,400.f});
        menuSection = 0;
        menuList.clear();
        menuList.push_back(makeMenuButton({250.f, 350.f}, 15, 2, "PLAY", true));
        menuList.push_back(makeMenuButton({250.f, 425.f}, 15, 2, "LEVEL DESIGN", false));
        menuList.push_back(makeMenuButton({250.f, 500.f}, 15, 2, "EXIT", false));
        logo = eFac.createImage(registry, {400, 275}, textures.get(Textures::Logo), true);
        registry.get<Draw>(logo).sprite.scale(0.35, 0.35);
        // add tutorial text to the first level
    } else if (level.compare("lvl1") == 0) {
        eFac.createRectangle(registry, {250, 750}, {400, 100}, sf::Color::Black, true);
        eFac.createText(registry, fonts, {260.f, 760.f}, "Use the arrow keys to move left and right.", 15, false);
        eFac.createText(registry, fonts, {260.f, 780.f}, "\'Space\' to jump.", 15, false);
        eFac.createText(registry, fonts, {260.f, 800.f}, "\'X\' to attack.", 15, false);
        eFac.createText(registry, fonts, {260.f, 820.f}, "Don't be afraid to swing at projectiles!", 15, false);
    } else if (level.compare("lvlDeath") == 0) {
    }

    // loads all the entities from the file with LevelHander::loadLevel()
    currentLevel = level;
    entt::entity e = lvlH.loadLevel("levels/" + level, textures, fonts, entList, registry);
    //makes sure the registry has returned a valid player entity
    if (registry.valid(e)) {
        playerID = e;
    }
    //initialises playersword variable by searching for its lavel
    if (!registry.valid(playerSword)) {
        const auto swordView = registry.view<Draw>();
        for (const entt::entity s : swordView) {
            if (swordView.get<Draw>(s).label == "sword") {
                swordView.get<Draw>(s).visible = false;
                playerSword = s;
            }
        }
    }
}

// any resources that need to be loaded into memory (textures and fonts) are intiliased here using the resourcehandler class.
bool Game::initTextures(std::vector<Entity> &eList) {
    try {
        textures.load(Textures::Player, "media/textures/att2.png");
        textures.load(Textures::Henchman, "media/textures/hench1.png");
        textures.load(Textures::Boss, "media/textures/Boss1.png");
        textures.load(Textures::Block, "media/textures/basicblock.png");
        textures.load(Textures::Landscape, "media/textures/vaporwave1.png");
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
    //initialise the first level of the game
    loadLevel("menu");
    loadLevel("menu");
    return 1;
}

//processes user input from the keyboard and mouse
void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        //on either a press or release event it sends the keycode and whether it has been pressed to the HandlePlayerInput function
        switch (event.type) {
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code, false);
            break;
            // in the case of a mouse event it more directly changes variables to indicate whether or not the button is pressed
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                mouseLClicked = true;
            } else {
                mouseRClicked = true;
            }

            handleMouseInput(event.mouseButton.button, true);
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                mouseLClicked = false;
            } else {
                mouseRClicked = false;
            }
            handleMouseInput(event.mouseButton.button, false);
            break;
            //move events also needed so that the user can drag paint in the designer
        case sf::Event::MouseMoved:
            handleMouseInput(event.mouseButton.button, false);
            break;
        case sf::Event::Closed:
            mWindow.close();
            break;
        }
    }
}

// when in the level designer users will want to add entities to the screen, this function allows for that
// it makes sure only one entity can occupy a coordinate and also creates the maps that are used to create level files 
void Game::addBlockToEditor(sf::Vector2f coords) {
    std::string sCoords = std::to_string(coords.x / 20) + "," + std::to_string(coords.y / 20);
    EntityFactory entFac;
    //make sure there is only one player entity placed
    if (numKey == 1 && playerPlaced) {
        return;
    }
    // if the coordinates are not in the map, the entity can be placed into the map and created on screen
    if (designMap.find(sCoords) == designMap.end()) {
        entt::entity e = entFac.createImage(registry, coords, textures.get(static_cast<Textures::ID>(numKey)), false);
        if (numKey == 1) {
            playerPlaced = true;
        }
        designMap.insert({sCoords, numKey});
        designEntityMap.insert({sCoords, e});
    }
}

// removes block from designer view and maps
void Game::removeBlockFromEditor(sf::Vector2f coords) {
    std::string sCoords = std::to_string(coords.x / 20) + "," + std::to_string(coords.y / 20);
    EntityFactory entFac;
    // makes sure to update whether the player is places
    if (designMap.find(sCoords) != designMap.end()) {
        if (designMap[sCoords] == 1) {
            playerPlaced = false;
        }
        //removes entity from both maps as well as the registry
        designMap.erase(sCoords);
        registry.remove_all(designEntityMap[sCoords]);
        designEntityMap.erase(sCoords);
    }
}

// allows the user to paint entities on the screen in designer mode
void Game::handleMouseInput(sf::Mouse::Button button, bool isPressed) {
    EntityFactory entFac;
    if (currentLevel == "designer") {
        // adds entities on left click and removes them on right click
        if (button == sf::Mouse::Left && (isPressed) || mouseLClicked) {
            sf::Vector2i m = sf::Mouse::getPosition();
            //since the mouse location is measured in relation to the viewport on the screen 
            // the cursor position must be mapped to actual world coordinates before it can be processed
            sf::Vector2f rm = mWindow.mapPixelToCoords(m, worldView);
            // std::cout << rm.x << " : " << rm.y << std::endl;
            addBlockToEditor({floor(rm.x / 20) * 20, floor(rm.y / 20) * 20});
        } else if (button == sf::Mouse::Right && (isPressed) || mouseRClicked) {
            sf::Vector2i m = sf::Mouse::getPosition();
            sf::Vector2f rm = mWindow.mapPixelToCoords(m, worldView);
            removeBlockFromEditor({floor(rm.x / 20) * 20, floor(rm.y / 20) * 20});
        }
    }
}

// handles keypresses
void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    // if using arrow keys it sets header variables that can be used ealsewhere
    if (key == sf::Keyboard::Up) {
        misMovingUp = isPressed;
    } else if (key == sf::Keyboard::Down) {
        misMovingDown = isPressed;
    } else if (key == sf::Keyboard::Left) {
        misMovingLeft = isPressed;
    } else if (key == sf::Keyboard::Right) {
        misMovingRight = isPressed;
    // formerlly used for testing
    } else if (key == sf::Keyboard::SemiColon) {
        // loadLevel("lvl1");
    } else if (key == sf::Keyboard::L) {
        // loadLevel("lvl3");
    // causes payer to jump
    } else if (key == sf::Keyboard::Space) {
        if (playerOnFloor) {
            playerJump = 1050;
            playerOnFloor = false;
        }
    } else if (key == sf::Keyboard::Escape) {
        loadLevel("menu");
    } else if (key == sf::Keyboard::P) {
        if(currentLevel=="designer"){
            LevelHandler lvlH;
            lvlH.saveLevel("lvlCustom", designMap);
        }
    } else if (key == sf::Keyboard::X) {
        mAttacking = isPressed;
    } else if (key == sf::Keyboard::Enter) {
        enterPressed = isPressed;
    // numver keys are used to change which entity is being painted in designer
    } else if (key == sf::Keyboard::Num1) {
        numKey = 1;
    } else if (key == sf::Keyboard::Num2) {
        numKey = 2;
    } else if (key == sf::Keyboard::Num3) {
        numKey = 3;
    } else if (key == sf::Keyboard::Num4) {
        numKey = 4;
    } else if (key == sf::Keyboard::Num5) {
        numKey = 5;
    } else if (key == sf::Keyboard::Num6) {
        numKey = 6;
    } else if (key == sf::Keyboard::Num7) {
        numKey = 7;
    } else if (key == sf::Keyboard::Num8) {
        numKey = 8;
    } else if (key == sf::Keyboard::Num9) {
        numKey = 9;
    }
    // std::cout << numKey << std::endl;
}

// creates the second part of the menu, the level select screen
void Game::changeToLevelSelect() {
    menuSection = 1;
    EntityFactory entFac;
    // clears registry then recreates essential entities
    menuList.clear();
    registry.clear();
    playerID = entFac.createPlayer(registry, textures, {0, 0});
    playerSword = entFac.createPlayerSword(registry, textures, {0, 0});

    //creates buttons for the various levels
    menuList.push_back(makeMenuButton({250.f, 250.f}, 15, 2, "LEVEL 1", true));
    menuList.push_back(makeMenuButton({250.f, 325.f}, 15, 2, "LEVEL 2", false));
    menuList.push_back(makeMenuButton({250.f, 400.f}, 15, 2, "LEVEL 3", false));
    menuList.push_back(makeMenuButton({250.f, 475.f}, 15, 2, "CUSTOM LEVEL", false));
    menuList.push_back(makeMenuButton({250.f, 550.f}, 15, 2, "BACK", false));
    menuClock.restart();
}


// the logic of the menu, allows user to select and option
void Game::updateMenu(sf::Time deltaTime) {
    // due to the speed of updates there needs to be a delay before a keypress is registered again
    if (menuClock.getElapsedTime().asSeconds() > 0.2) {
        // the text entities in the menu buttons are stored in a vector
        // this is looped through and the selected menu text gets a different colour
        maxMenuState = menuList.size();
        // std::cout << "menSize" << maxMenuState << std::endl;
        if (misMovingDown) {
            menuState++;
            menuClock.restart();
        }
        if (misMovingUp) {
            menuState--;
            menuClock.restart();
        }
        // std::cout << menuState << std::endl;
        if (menuState >= maxMenuState) {
            menuState = 0;
        }
        if (menuState < 0) {
            menuState = maxMenuState - 1;
        }
    }

    for (int i = 0; i < menuList.size(); i++) {
        if (i == menuState) {
            registry.get<Text>(menuList[i]).text.setOutlineColor(sf::Color::Red);
        } else {
            registry.get<Text>(menuList[i]).text.setOutlineColor(sf::Color::Black);
        }
    }
    // chooses an option
    if (menuClock.getElapsedTime().asSeconds() > 0.2) {
        if (enterPressed) {
            menuClock.restart();
            // if it is in the main menu this switch is used, in the level select menu the other is used
            if (menuSection == 0) {
                switch (menuState) {
                case 0:
                    // currentLevel = "lvl1";
                    // loadLevel("lvl1");
                    changeToLevelSelect();
                    return;
                    break;
                case 1:
                    currentLevel = "designer";
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
            } else if (menuSection == 1) {
                switch (menuState) {
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
}

// initialises viewport
// since monitors have different aspect ratios, this creates a viewport with 'black bars' for different aspect ratios
// some of this function was taken from the sfml forums https://en.sfml-dev.org/forums/index.php
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

//sets up and plays a music stream
bool Game::initMusic() {
    if (music.openFromFile("media/Music/bigmusic.ogg")) {
        music.setLoop(true);
        music.play();
        return true;
    }
    return false;
}


//main run loop of the game
void Game::run() {
    initWindow();
    initTextures(entList);
    initECS();
    initMusic();

    //     return -1; // error
    // music.play();
    sf::Vector2f ploc = registry.get<Draw>(playerID).sprite.getPosition();
    // std::cout << ploc.x/20 << " : " << ploc.y/20 << std::endl;
    // Entity o(textures.get(Textures::Landscape), -600.f, -200.f, false);
    //todo make this ecs entity instead
    //creates background
    Entity o(textures.get(Textures::Landscape), ploc.x, ploc.y, false);
    o.setOrigin(o.getBoundingBox().width / 2, o.getBoundingBox().height / 2);
    o.scale(0.5f, 0.5f);

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    //a loop that syncronises updates with the clock instead of with frames, values and multiplied by the elapsed time so that they are only updated a proprtional amout to the time passed
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
        o.setPostion(worldView.getCenter());
        o.draw(mWindow);

        //kills the player if they drop off the map
        sf::Vector2f plo = registry.get<Draw>(playerID).sprite.getPosition();
        if (plo.y > 1500) {
            playerDeath();
        }
        // std::cout << plo.x/20 << " : " << plo.y/20 << std::endl;

        //sets the location of the worldview (camera) depedning on the level
        if (currentLevel != "menu" && currentLevel != "designer") {
            worldView.setCenter(registry.get<Draw>(playerID).sprite.getPosition());
        } else if (currentLevel != "designer") {
            worldView.setCenter({400.f, 400.f});
        }
        mWindow.setView(worldView);

        //iterates through all entities with an animation component and updates their animation
        const auto animView = registry.view<Animation, Draw, Movement>();
        for (const entt::entity a : animView) {
            if (animView.get<Animation>(a).animClock.getElapsedTime().asMilliseconds() > animView.get<Animation>(a).idleTime * 10) {
                animView.get<Animation>(a).animClock.restart();
                std::vector<int> textureList = animView.get<Animation>(a).textureList;
                sf::Sprite sp = animView.get<Draw>(a).sprite;
                sf::Vector2f vel = animView.get<Movement>(a).velocity;
                if (animView.get<Animation>(a).currentTexture == 0 && vel.y >= 0 && vel.x != 0) {
                    sp.setTexture(textures.get(static_cast<Textures::ID>(textureList[1])));
                    animView.get<Animation>(a).currentTexture = 1;
                } else if (animView.get<Animation>(a).currentTexture == 1 && vel.y >= 0 && vel.x != 0) {
                    sp.setTexture(textures.get(static_cast<Textures::ID>(textureList[0])));
                    animView.get<Animation>(a).currentTexture = 0;
                } else if (animView.get<Animation>(a).currentTexture == 2 && animView.get<Animation>(a).attackClock.getElapsedTime().asSeconds() > 0.3) {
                    animView.get<Animation>(a).canAttack = true;
                    registry.get<Animation>(playerID).attackClock.restart();
                    sp.setTexture(textures.get(static_cast<Textures::ID>(textureList[0])));
                    animView.get<Animation>(a).currentTexture = 0;
                    if (a == playerID) {
                        registry.get<Draw>(playerSword).visible = false;
                    }
                }
                animView.get<Draw>(a).sprite = sp;
            }
        }

        //makes sure the players sword changes location as the player turns
        const auto view = registry.view<Draw>();
        sf::Vector2f pPos = view.get<Draw>(playerID).sprite.getPosition();
        if (view.get<Draw>(playerSword).facing) {
            view.get<Draw>(playerSword).sprite.setPosition(pPos.x + 26, pPos.y + 8);
        } else {
            view.get<Draw>(playerSword).sprite.setPosition(pPos.x - 23, pPos.y + 8);
        }

        // sSprite.setPosition();
        // registry.get<Draw>(playerSword).sprite = sSprite;

        //draws all visable entities with the Draw component
        for (const entt::entity e : view) {
            if (!(e == playerID && currentLevel.substr(0, 3) != "lvl")) {
                if (view.get<Draw>(e).visible) {

                    sf::Sprite spri = view.get<Draw>(e).sprite;
                    mWindow.draw(spri);
                }
            } else {
                // mWindow.draw(registry.get<Draw>(logo).sprite);
            }
        }

        //draws all visible shapes with the DrawShape component
        const auto view3 = registry.view<DrawShape>();
        if (!view3.empty()) {
            for (const entt::entity e : view3) {
                sf::RectangleShape re = view3.get<DrawShape>(e).rect;
                mWindow.draw(re);
            }
        }

        //draws all visible text with <Text> component
        const auto view2 = registry.view<Text>();
        if (!view2.empty()) {
            for (const entt::entity e : view2) {
                sf::Text text = view2.get<Text>(e).text;
                mWindow.draw(text);
            }
        }

        mWindow.display();
    }
}

//shortcut for creating menu buttons, rectangle of plantforms with text on to[]
entt::entity Game::makeMenuButton(sf::Vector2f pos, int length, int height, std::string text, bool primary) {
    EntityFactory entFac;
    for (int i = 0; i <= height; i++) {
        for (int x = 0; x <= length; x++) {
            entFac.createPlatform(registry, textures, {pos.x + (x * 20), pos.y + (i * 20)});
        }
    }
    entt::entity e = entFac.createText(registry, fonts, {pos.x + (length * 10), pos.y + (height * 10)}, text, 40, true);
    if (primary) {
        registry.get<Text>(e).text.setOutlineColor(sf::Color::Red);
    }
    return e;
}

// creates entities required for designer view
void Game::initDesigner() {
    sf::Vector2f pos = worldView.getCenter();
    ;
    playerPlaced = false;
    designMap.clear();
    designEntityMap.clear();
    // pos.y += (worldView.getSize().y/;
    EntityFactory entFac;

    textList.clear();
    imageList.clear();
    rectList.clear();
    for (int i = 1; i <= 9; i++) {
        entt::entity e = entFac.createImage(registry, {pos.x + 75 * i, pos.y}, textures.get(static_cast<Textures::ID>(i)), false);
        imageList.push_back(e);
        entt::entity t = entFac.createText(registry, fonts, {(pos.x + 75 * i) - 6, pos.y + 35}, std::to_string(i), 25, true);
        textList.push_back(t);
        entt::entity r = entFac.createRectangle(registry, {(pos.x + 75 * i) - 15, pos.y - 10}, {65.f, 70.f}, sf::Color::Red, false);
        rectList.push_back(r);
    }
    rectList.push_back(entFac.createRectangle(registry, {(pos.x + 750), pos.y - 10}, {480.f, 60.f}, sf::Color::Black, true));
    textList.push_back(entFac.createText(registry, fonts, {(pos.x + 750) - 6, pos.y + 35}, "Press P to save your creation to a file", 15, false));
    textList.push_back(entFac.createText(registry, fonts, {(pos.x + 750) - 6, pos.y + 55}, "Left click to place entities, right click to remove them", 15, false));
    textList.push_back(entFac.createText(registry, fonts, {(pos.x + 750) - 6, pos.y + 75}, "Switch between entities using the number keys", 15, false));
    textList.push_back(entFac.createText(registry, fonts, {(pos.x + 750) - 6, pos.y + 95}, "Use the arrow keys to navigate the level", 15, false));
}

// makes sure that the deisgner HUD stays in the same place relative to the camera even though it is moving
void Game::updateDesignerHUD() {
    entt::basic_view recView = registry.view<DrawShape>();
    sf::Vector2f pos = worldView.getCenter();
    pos.x -= worldView.getSize().x / 2.1;
    pos.y -= worldView.getSize().y / 2.1;
    for (int i = 0; i < 9; i++) {
        registry.get<Draw>(imageList[i]).sprite.setPosition({pos.x + 75 * i, pos.y});
        registry.get<Text>(textList[i]).text.setPosition({(pos.x + 75 * i) - 6, pos.y + 35});
        registry.get<DrawShape>(rectList[i]).rect.setPosition({(pos.x + 75 * i) - 15, pos.y - 10});
    }
    registry.get<DrawShape>(rectList[9]).rect.setPosition({(pos.x + 285), pos.y + 370});
    registry.get<Text>(textList[9]).text.setPosition({(pos.x + 300), pos.y + 375 - 4});
    registry.get<Text>(textList[10]).text.setPosition({(pos.x + 300), pos.y + 385 - 3});
    registry.get<Text>(textList[11]).text.setPosition({(pos.x + 300), pos.y + 395 - 2});
    registry.get<Text>(textList[12]).text.setPosition({(pos.x + 300), pos.y + 405 - 1});
}


// update function for when in designer mode, sets the selected entity to have a red outline and moves the camera if the arrow keys are being pressed
void Game::updateDesigner(sf::Time deltaTime) {
    entt::basic_view recView = registry.view<DrawShape>();
    for (int i = 0; i < 9; i++) {
        sf::RectangleShape r = recView.get<DrawShape>(rectList[i]).rect;
        if ((i + 1) == numKey) {
            r.setOutlineColor(sf::Color::Red);
            r.setOutlineThickness(3);
        } else {
            r.setOutlineColor(sf::Color::Black);
            r.setOutlineThickness(1);
        }
        recView.get<DrawShape>(rectList[i]).rect = r;
    }
    int viewVelocity = 200;
    sf::Vector2f viewV = {0.f, 0.f};

    if (misMovingUp) {
        viewV.y -= viewVelocity;
    }
    if (misMovingDown) {
        viewV.y += viewVelocity;
    }
    if (misMovingLeft) {
        viewV.x -= viewVelocity;
    }
    if (misMovingRight) {
        viewV.x += viewVelocity;
    }
    worldView = mWindow.getView();
    worldView.move(viewV * deltaTime.asSeconds());
    mWindow.setView(worldView);
    updateDesignerHUD();
}

// update function for when a level is being played
void Game::updateLevel(sf::Time deltaTime) {
    //jump mechanics for the player
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
        //flips the player and their sword when they start moving in another direction
        movement.x -= 370.f;
        entt::entity plandsw[2] = {playerID, playerSword};
        for (auto e : plandsw) {
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
        entt::entity plandsw[2] = {playerID, playerSword};
        for (auto e : plandsw) {
            if (!registry.get<Draw>(e).facing) {
                sf::Sprite sp = registry.get<Draw>(e).sprite;
                sf::IntRect ir = sp.getTextureRect();
                sp.setTextureRect(sf::IntRect(0, 0, -ir.width, ir.height));
                registry.get<Draw>(e).sprite = sp;
                registry.get<Draw>(e).facing = true;
            }
        }
    }
    //changes the texture of an entity to the attack texture when needed
    if (mAttacking && registry.get<Animation>(playerID).canAttack && registry.get<Animation>(playerID).attackClock.getElapsedTime().asSeconds() > 0.55) {
        registry.get<Animation>(playerID).canAttack = false;
        registry.get<Animation>(playerID).attackClock.restart();
        std::vector<int> textureList = registry.get<Animation>(playerID).textureList;
        sf::Sprite sp = registry.get<Draw>(playerID).sprite;
        sp.setTexture(textures.get(static_cast<Textures::ID>(textureList[2])));
        registry.get<Draw>(playerID).sprite = sp;
        registry.get<Animation>(playerID).currentTexture = 2;
        registry.get<Draw>(playerSword).visible = true;
    }

    // for entities with paths this makes sure that they walk between them and face the correct directions while doing it
    const auto aiView = registry.view<Movement, AI, Draw>();
    for (const entt::entity e : aiView) {

        sf::Vector2f currentPos = aiView.get<Draw>(e).sprite.getPosition();
        // std::cout << "pos " << currentPos.x << std::endl;
        std::vector<sf::Vector2f> path = aiView.get<AI>(e).path;
        // std::cout << "Pth: " << path.size() <<  std::endl;
        if (path.size() > 0) {
            sf::Sprite sp = aiView.get<Draw>(e).sprite;
            sf::IntRect ir = sp.getTextureRect();
            if (currentPos.x > path[1].x) {
                aiView.get<Movement>(e).velocity.x = -enemyMoveSpeed;
                if (aiView.get<AI>(e).facing) {
                    aiView.get<AI>(e).facing = false;

                    sp.setTextureRect(sf::IntRect(ir.width, 0, -ir.width, ir.height));
                }
            } else if (currentPos.x < path[0].x) {

                aiView.get<Movement>(e).velocity.x = enemyMoveSpeed;
                if (!aiView.get<AI>(e).facing) {
                    aiView.get<AI>(e).facing = true;
                    sp.setTextureRect(sf::IntRect(0, 0, -ir.width, ir.height));
                }
            } else if (aiView.get<Movement>(e).velocity.x == 0) {
                if (!aiView.get<AI>(e).facing) {
                    aiView.get<AI>(e).facing = true;
                    sp.setTextureRect(sf::IntRect(0, 0, -ir.width, ir.height));
                }
                aiView.get<Movement>(e).velocity.x = enemyMoveSpeed;
            }
            aiView.get<Draw>(e).sprite = sp;
        }
    }
    // iterates through ranged attackers and has the fire prjectiles at a set interval
    const auto shootView = registry.view<AttackRanged, Animation, Draw>();
    EntityFactory eF;
    for (const entt::entity sh : shootView) {
        if (shootView.get<Animation>(sh).attackClock.getElapsedTime().asSeconds() > 3 && sh != playerID) {
            shootView.get<Animation>(sh).attackClock.restart();
            sf::Vector2f pos = shootView.get<Draw>(sh).sprite.getPosition();
            pos.y += 17;
            eF.createBullet(registry, textures, pos, -150);
            if (shootView.get<Draw>(sh).label == "boss") {
                eF.createBullet(registry, textures, pos, 150);
            }
        }
    }

    // kills the projectiles after they have been alive a certain amount of time
    const auto bulletView = registry.view<Draw>();
    for (const entt::entity bu : bulletView) {
        if (bulletView.get<Draw>(bu).label.compare("bullet") == 0) {
            // std::cout << "BU " << bulletView.get<Draw>(bu).aliveTime.getElapsedTime().asSeconds() << std::endl;
            if (bulletView.get<Draw>(bu).aliveTime.getElapsedTime().asSeconds() > 3) {
                registry.remove_all(bu);
            }
        }
    }

    // checks for collisions with the player's sword
    // destroying / damaging entities it contacts
    const auto killView = registry.view<Draw, Physics>();
    if (killView.get<Draw>(playerSword).visible) {
        for (const entt::entity j : killView) {
            sf::FloatRect jBounds = killView.get<Draw>(j).sprite.getGlobalBounds();
            sf::FloatRect swordBounds = killView.get<Draw>(playerSword).sprite.getGlobalBounds();
            if (jBounds.intersects(swordBounds) && (killView.get<Draw>(j).label == "enemy" || killView.get<Draw>(j).label == "bullet")) {
                // view2.get<Draw>(j).visible = false;
                registry.remove_all(j);
            } else if (jBounds.intersects(swordBounds) && killView.get<Draw>(j).label == "boss") {
                bossHealth--;
                std::cout << bossHealth << std::endl;
                if (bossHealth <= 0) {
                    playerWin();
                    registry.remove_all(j);
                }
            }
        }
    }


    // The collision system
    // checks to see whether the next move that an entity will make will intersect along either axis, it then changes the movement so that it goes just far enough to touch but not intersect
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
        if (e == playerID) {
            for (const entt::entity j : view2) {
                std::string jLabel = view2.get<Draw>(j).label;
                sf::FloatRect jBounds = view2.get<Draw>(j).sprite.getGlobalBounds();

                if (j != e && view2.get<Physics>(j).hasCollision) {
                    if (jLabel == "bullet" || jLabel == "enemy" || jLabel == "boss") {
                        if (jBounds.intersects(view.get<Draw>(e).sprite.getGlobalBounds())) {
                            playerDeath();
                            break;
                        }
                    } else {
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


void Game::updateDeath(sf::Time deltaTime) {
    std::cout << "udeath" << std::endl;
}

// main update loops, checks if the player is dead or has completed the level
// mostly calls the other update functions depending on the type of level
void Game::update(sf::Time deltaTime) {
    if (kill) {
        // currentLevel = "death";
        sleep(sf::seconds(1));
        nextStep = currentLevel;
        loadLevel(currentLevel);
        kill = false;
    }
    if (win) {
        sleep(sf::seconds(1));
        loadLevel("menu");
        win = false;
    }
    if (currentLevel.compare("menu") == 0) {
        updateMenu(deltaTime);
    } else if (currentLevel.compare("designer") == 0) {
        updateDesigner(deltaTime);
    } else if (currentLevel.compare("death") == 0) {
        updateDeath(deltaTime);
    } else {
        updateLevel(deltaTime);
    }
}

void Game::playerDeath() {
    EntityFactory ef;
    entt::entity e = ef.createText(registry, fonts, registry.get<Draw>(playerID).sprite.getPosition(), "YOU DIED", 60, true);
    mWindow.draw(registry.get<Text>(e).text);
    registry.get<Physics>(playerID).hasCollision = false;
    kill = true;
    // if(!deathLoading){
    // deathLoading = true;
    // currentLevel = "death";
    // nextStep = currentLevel;
    // loadLevel("lvlDeath");
}

void Game::playerWin() {
    EntityFactory ef;
    entt::entity e = ef.createText(registry, fonts, registry.get<Draw>(playerID).sprite.getPosition(), "LEVEL COMPLETE", 60, true);
    mWindow.draw(registry.get<Text>(e).text);
    registry.get<Physics>(playerID).hasCollision = false;
    win = true;
    // currentLevel = "win";
    // nextStep = "menu";
    // loadLevel("win");
}
