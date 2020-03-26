#ifndef LEVELHANDLE
#define LEVELHANDLE

#include <SFML/Graphics.hpp>
#include "ResourceHandler.h"
#include <iostream>
#include <fstream>


class LevelHandler{
    public:
        LevelHandler();
        int loadLevel(std::string fileName);

    private:
        int i;


};

#endif