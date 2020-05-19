#ifndef COMP_DRAW
#define COMP_DRAW

#include <SFML/Graphics.hpp>

struct Draw {
  sf::Texture texture;
  sf::Sprite sprite;
  bool visible = false;
  sf::Vector2f location;
  bool facing;
  std::string label = "";
  sf::Clock aliveTime;
};

#endif
