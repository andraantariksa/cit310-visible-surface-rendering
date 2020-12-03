#include "Color.hpp"

#include <SFML/Graphics.hpp>

sf::Color SFColorFromdouble3(const float color[3])
{
	return std::move(sf::Color(
		(sf::Uint8)(color[0] * 255.0),
		(sf::Uint8)(color[1] * 255.0),
		(sf::Uint8)(color[2] * 255.0)
	));
};