#include "Windows.hpp"

#include <Windows.h>

namespace windows
{
	namespace window
	{
		void maximise(sf::WindowHandle handle)
		{
			ShowWindow(handle, SW_SHOWMAXIMIZED);
		}

		void normal(sf::WindowHandle handle)
		{
			ShowWindow(handle, SW_SHOWNORMAL);
		}

		void minimise(sf::WindowHandle handle)
		{
			ShowWindow(handle, SW_MINIMIZE);
		}
	}
}