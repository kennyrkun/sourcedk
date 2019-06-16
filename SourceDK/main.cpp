#include "AppEngine.hpp"

#include "MainMenuState.hpp"

#include <SFUI/Theme.hpp>

int main()
{
	SFUI::Theme::loadFont("resources/interface/tahoma.ttf");
	SFUI::Theme::loadTexture("resources/interface/vgui.png");
	SFUI::Theme::textCharacterSize = 11;
	SFUI::Theme::click.textColor = sf::Color::White;
	SFUI::Theme::click.textColorHover = sf::Color::White;
	SFUI::Theme::click.textColorFocus = sf::Color::White;
	SFUI::Theme::input.textColor = sf::Color::White;
	SFUI::Theme::input.textColorHover = sf::Color::White;
	SFUI::Theme::input.textColorFocus = sf::Color::White;
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
