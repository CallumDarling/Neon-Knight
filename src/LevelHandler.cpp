#include "headers/LevelHandler.h"

LevelHandler::LevelHandler(){
    //
}

int LevelHandler::loadLevel(std::string fileName,
                            ResourceHandler<sf::Texture, Textures::ID>& textures
                            ,std::vector<Entity>& entList){

    std::ifstream file(fileName);
    if(file){
        int stage = 0;
       for( std::string line; std::getline( file, line ); ){
           
           std::vector<std::string> strings;
            // std::cout << stage << line << std::endl;
            std::stringstream sS(line);
            while(sS.good()){
                std::string substr;
                std::getline(sS, substr, ',');
                strings.push_back(substr);
                std::cout <<  substr << std::endl;
            }

            if( line == "###"){
                stage++;
            }else if(stage == 0){
                try{
                    Entity e(textures.get(Textures::Block), std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f, false);
                    entList.push_back(e);
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                }
            }else if(stage == 1){
                 try{
                    Entity e(textures.get(Textures::Boss), std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f, true);
                    entList.push_back(e);
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                }

            }else if(stage == 2){
                 try{
                    Entity e(textures.get(Textures::Henchman), std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f, true);
                    entList.push_back(e);
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                }

            }else if(stage == 3){

            }
        }     

        return 1;
    }else{
        return 0;
    }

}