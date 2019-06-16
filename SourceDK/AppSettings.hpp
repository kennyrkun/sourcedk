#ifndef APP_SETTINGS_HPP
#define APP_SETTINGS_HPP

struct AppSettings
{
	bool SFUIDebug = false;
	bool allowStatTracking = false;
	bool useAnimations = false;
	int animationScale = 1;

	struct Window
	{
		bool verticalSync = true;
//		bool useSFUIDecorations = true;
		int width = 310;
		int	height = 500;
	} window;
};

#endif // !APP_SETTINGS_HPP
