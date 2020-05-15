#ifndef COMP_DRAW
#define COMP_DRAW

#include <SFML/Graphics.hpp>

struct Draw {
  sf::Texture texture;
  sf::Sprite sprite;
  bool visible;
  sf::Vector2f location;
  bool facing;
};

#endif
