#ifndef COMP_SHDRAW
#define COMP_SHDRAW

#include <SFML/Graphics.hpp>

struct DrawShape {
  sf::RectangleShape rect;
  bool visible;
  sf::Vector2f location;
};

#endif
