#include "AppEngine.hpp"

#include "MainMenuState.hpp"

#include <SFUI/Theme.hpp>

int main()
{
	SFUI::Theme::loadFont("resources/interface/tahoma.ttf");
	SFUI::Theme::loadTexture("resources/interface/vgui.png");
	SFUI::Theme::textCharacterSize = 11;

	sf::Color white = sf::Color::White;
	SFUI::Theme::click = { white, white, white };
	SFUI::Theme::input = { white, white, white };
	SFUI::Theme::label = { white, white, white };
//	SFUI::Theme::label.textColor = sf::Color(210, 190, 86);

	SFUI::Theme::PADDING = 2.f;

	AppSettings settings;
	settings.window.verticalSync = false;

	AppEngine app;

	app.Init("SourceDK", settings);

	app.PushState(new MainMenuState);

	while (app.Running())
	{
		app.HandleEvents();
		app.Update();
		app.Draw();
	}

	app.Cleanup();

	return EXIT_SUCCESS;
}
