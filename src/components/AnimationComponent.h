#ifndef COMP_ANIMATION
#define COMP_ANIMATION

#include <SFML/Graphics.hpp>

struct Animation{
    std::vector<int> textureList;
    float idleTime;
    float transitionTime;
    sf::Clock animClock;
    sf::Clock attackClock;
    bool canAttack = true;
    int currentTexture = 0;
};

#endif