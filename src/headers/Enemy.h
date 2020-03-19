#ifndef ENEMY
#define ENEMY

# include "Entity.h"

class Enemy : public Entity{
    public:
        enum Type{
            Henchman,
            Gunman,
            Boss
        };
    public:
        explicit Enemy(Type type);

    private:
        Type mType;
};

#endif