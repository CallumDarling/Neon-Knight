#ifndef COMP_AI
#define COMP_AI

#include <SFML/Graphics.hpp>

struct AI{
    std::vector<sf::Vector2f> path;
    std::vector<sf::Vector2f> vision;
    bool canSeePlayer;
};

#endif