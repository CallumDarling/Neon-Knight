#ifndef COMP_ATTACKRANGED
#define COMP_ATTACKRANGED

#include <SFML/Graphics.hpp>

struct AttackRanged{
    std::string projectile;
    int fireCoolDown;
    int projDamage;
};

#endif