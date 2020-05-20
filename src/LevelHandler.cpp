#include "headers/LevelHandler.h"

LevelHandler::LevelHandler(){
    //
}


//this function take a map with the string coordinates of entities and their ID and converts it into a format which can be saved into a file and loaded
int LevelHandler::saveLevel(std::string filename, std::map<std::string, int> designMap){
    std::ofstream file("levels/"+filename);
    //createa a blank string for each type of entity that is going to be stored
    // iterate through the map appending entities to the appropriate string 
    std::vector<std::string> types = {"","","","","","","","","",""};
    for (auto const& x : designMap){
        std::cout << "append" << std::endl;
        std::stringstream ss(x.first);
        std::vector<std::string> v; 
  
        //as some entities need to be stored with paths to walk we need to convert the strings back into float coordinates
        while (ss.good()) { 
            std::string substr; 
            getline(ss, substr, ','); 
            v.push_back(substr); 
        } 
        sf::Vector2f coords = sf::Vector2f(std::stof(v[0]),std::stof(v[1]));
        std::string adjustedF = std::to_string(coords.x+5)+","+std::to_string(coords.y);
        std::string adjustedB = std::to_string(coords.x-5)+","+std::to_string(coords.y);;

        //appending the coordinates to the appropriate string
        switch (x.second)
        {
        case 2:
            types[x.second].append(x.first+","+adjustedB+","+adjustedF+"\n");
            break;
        case 3:
            types[x.second].append(x.first+","+adjustedB+","+adjustedF+"\n");
            break;
        case 4:
            types[x.second].append(x.first+",0"+"\n");
            break;
        case 5:
            types[x.second].append(x.first+","+adjustedB+","+adjustedF+"\n");
            break;
        default:
            types[x.second].append(x.first+"\n");
            break;
        }
        
    }
    //if there is no player entity in the map create one
    if(file){
        // file << "###";
        if(types[1]==""){

            types[1]="20,15\n";

        }
        // loop through the array of strings outputing each set off coordinates into a line in the file
        // different entities are seperated by a line beginning with ###
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

// takes in a level file and initialises all the entities stored there
entt::entity LevelHandler::loadLevel(std::string fileName,
                            ResourceHandler<sf::Texture, Textures::ID>& textures,
                            ResourceHandler<sf::Font, Fonts::ID>& fonts,
                            std::vector<Entity>& entList,
                            entt::registry& reg){

    std::ifstream file(fileName);
    EntityFactory entFac;
    if(fileName.compare("levels/menu")==0){
        // entFac.createText(reg,fonts,{400.f,400.f},"Text", 40);
    }
    entt::entity newPlayer;
    if(file){
        //access the file and split it by line
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
            //every time you hit the ### delimeter, iterate a stage so the type of entity being created changes
            //which entity is being created is self evident from the function names.
            if( line.substr(0,3) == "###"){
                stage++;
            }else if(stage == 1){
                try{
                    //create the player and their sword
                    newPlayer = entFac.createPlayer(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f});
                    entFac.createPlayerSword(reg,textures, {(std::stoi(strings[0])*20.f)+26, (std::stoi(strings[1])*20.f)+18});

                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                }
            }else if(stage == 6){
                try{
                    entFac.createBlock(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f});
                    
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                }
            }else if(stage == 3){
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

            }else if(stage == 5){
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
            }else if(stage == 8){
                try{
                    entFac.createPlatform(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f});
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                    
                }
            }else if(stage == 7){
                try{
                    // Entity e(textures.get(Textures::Henchman), std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f, true);
                    // entList.push_back(e);
                    entFac.createLadder(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f});
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                    
                }
            }else if(stage == 9){
                try{
                    // Entity e(textures.get(Textures::Henchman), std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f, true);
                    // entList.push_back(e);
                    entFac.createDoor(reg,textures, {std::stoi(strings[0])*20.f, std::stoi(strings[1])*20.f});
                }catch(const std::exception& ex){
                    std::cout << "levelfail" << std::endl;
                    
                }
            }
        }     

        return newPlayer;
    }else{
        return newPlayer;
    }

}