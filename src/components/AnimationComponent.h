#ifndef COMP_ANIMATION
#define COMP_ANIMATION

#include <SFML/Graphics.hpp>

struct Animation{
    std::vector<std::string> textureList;
    float idleTime;
    float transitionTime;
};

#endif