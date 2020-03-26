#include "headers/LevelHandler.h"

LevelHandler::LevelHandler(){
    //
}

int LevelHandler::loadLevel(std::string fileName){
    std::ifstream file(fileName);
    if(file){
        for( std::string line; std::getline( file, line ); ){
            std::cout << line << std::endl;
        }     

        return 1;
    }else{
        return 0;
    }

}