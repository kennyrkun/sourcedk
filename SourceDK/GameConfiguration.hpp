#ifndef GAME_CONFIGURATION_HPP
#define GAME_CONFIGURATION_HPP

#include "Games.hpp"
#include "Engine.hpp"

#include <string>

class GameConfiguration
{
	std::string name;
	
	std::string path;

	Engine engine;
};

#endif // !GAME_CONFIGURATION_HPP