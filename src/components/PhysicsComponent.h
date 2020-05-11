#ifndef COMP_PHYSICS
#define COMP_PHYSICS

#include <SFML/Graphics.hpp>

struct Physics{
    bool hasGrav;
    float gravMultiplier;
    bool hasCollision;
    std::vector<std::string> collisionExceptions;
};

#endif