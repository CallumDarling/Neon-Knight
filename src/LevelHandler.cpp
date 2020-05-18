#include "headers/LevelHandler.h"

LevelHandler::LevelHandler(){
    //
}

int LevelHandler::saveLevel(std::string filename, std::map<std::string, int> designMap){
    std::ofstream file("levels/"+filename);
    std::vector<std::string> types = {"","","","","","","","",""};
    for (auto const& x : designMap){
        std::cout << "append" << std::endl;
        types[x.second].append(x.first+"\n");
    }
    if(file){
         for(auto const& y : types){
            file << y << "###" << "\n";
            std::cout << y << "###" << "\n";
        }
    }else
    {
        std::cout << "couldnt access file for writing" << std::endl;
        return 0;
    }
    return 1;
}


int LevelHandler::loadLevel(std::string fileName,
                            ResourceHandler<sf::Texture, Textures::ID>& textures,
                            ResourceHandler<sf::Font, Fonts::ID>& fonts,
                            std::vector<Entity>& entList,
                            entt::registry& reg){

    std::ifstream file(fileName);
    EntityFactory entFac;
    if(fileName.compare("levels/menu")==0){
        // entFac.createText(reg,fonts,{400.f,400.f},"Text", 40);
    }
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

            if( line.substr(0,3) == "###"){
                stage++;
            }else if(stage == 0){
                try{
                    entFac.createBlock(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f});
                    
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                }
            }else if(stage == 1){
                 try{
                    entFac.createBoss(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f}, 
                    {{std::stof(strings[2])*20.f,std::stof(strings[3])*20.f},{std::stof(strings[4])*20.f,std::stof(strings[5])*20.f}});
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                }

            }else if(stage == 2){
                 try{
                    entFac.createHench(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f}, 
                    {{std::stof(strings[2])*20.f,std::stof(strings[3])*20.f},{std::stof(strings[4])*20.f,std::stof(strings[5])*20.f}});
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                    
                }

            }else if(stage == 3){
                try{
                    entFac.createBrute(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f}, 
                    {{std::stof(strings[2])*20.f,std::stof(strings[3])*20.f},{std::stof(strings[4])*20.f,std::stof(strings[5])*20.f}});
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                    
                }
            }else if(stage == 4){
                try{
                    entFac.createGun(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f},std::stoi(strings[2]));
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                    
                }
            }else if(stage == 5){
                try{
                    entFac.createPlatform(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f});
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                    
                }
            }else if(stage == 6){
                try{
                    // Entity e(textures.get(Textures::Henchman), std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f, true);
                    // entList.push_back(e);
                    entFac.createLadder(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f});
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                    
                }
            }else if(stage == 7){
                try{
                    // Entity e(textures.get(Textures::Henchman), std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f, true);
                    // entList.push_back(e);
                    entFac.createDoor(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f});
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                    
                }
            }
        }     

        return 1;
    }else{
        return 0;
    }

}