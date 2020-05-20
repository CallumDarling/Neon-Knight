CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu++17 -g -fsanitize=address
LDFLAGS =  -fsanitize=address -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system 

SRC = main.cpp Game.cpp Entity.cpp LevelHandler.cpp EntityFactory.cpp
OBJ = $(SRC:.cc=.o)
EXEC = Neon-Knight

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -std=c++17 $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)