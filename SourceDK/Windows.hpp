/////////////////////////////////////
// This was designed just to avoid having Windows.h included in any files that needed these functions.
/////////////////////////////////////

#ifndef WINDOWS_HPP
#define WINDOWS_HPP

#include <SFML/Window/WindowHandle.hpp>

namespace windows
{
	namespace window
	{
		void maximise(sf::WindowHandle handle);

		void normal(sf::WindowHandle handle);

		void minimise(sf::WindowHandle handle);
	}
}

#endif // !WINDOWS_HPP