#include "GameConfigurationEditState.hpp"

#include "DisabledButton.hpp"
#include "VGUI.hpp"
#include "SettingsParser.hpp"

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

void GameConfigurationEditState::Init(AppEngine* appEngine)
{
	app = appEngine;

	window = new sf::RenderWindow(sf::VideoMode(310, 150), "Edit Configuration:");

	std::cout << "Initialising GameConfigurationEditState." << std::endl;

	SFUI::Theme::windowBgColor = app->windowDecorations.colors.windowBackground;

	menu = new SFUI::Menu(*app->window);
	menu->setPosition(sf::Vector2f(app->windowDecorations.sizes.left + 10, app->windowDecorations.sizes.top + app->windowDecorations.sizes.titlebar + 8));

	if (!app->sharedStateData.empty())
	{
		oldName = app->sharedStateData["gameName"];
		name = oldName;
		load("./sourcedk/gameconfigurations/" + name + "/game.conf");
		app->sharedStateData.clear();
	}

	gameName = new SFUI::InputBox;
	gameName->setText(name);
	gameDirectory = new SFUI::InputBox;
	gameDirectory->setText(directory);

	buildHomeInterface(&app->windowDecorations, *menu);
}

void GameConfigurationEditState::Cleanup()
{
	std::cout << "Cleaning up GameConfigurationEditState." << std::endl;

	delete menu;
	delete window;

	std::cout << "GameConfigurationEditState cleaned up." << std::endl;
}

void GameConfigurationEditState::Pause()
{
	std::cout << "GameConfigurationEditState paused." << std::endl;
}

void GameConfigurationEditState::Resume()
{
	std::cout << "GameConfigurationEditState resumed." << std::endl;
}

void GameConfigurationEditState::HandleEvents()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			window->close();
			app->PopState();
			return;
		}

		int id = menu->onEvent(event);
		switch (id)
		{
		case CALLBACKS::OK:
		{
			name = gameName->getText();
			directory = gameDirectory->getText();

			if (!fs::exists("./sourcedk/gameconfigurations/" + name + "/game.conf"))
			{
				std::cerr << "game configuration file not found, attempting to create one." << std::endl;
				try
				{
					fs::create_directory("./sourcedk/gameconfigurations/" + name);
				}
				catch (std::exception &e)
				{
					std::cerr << e.what() << std::endl;
				}

				std::ofstream createFile("./sourcedk/gameconfigurations/" + name + "/game.conf", std::ios::app);
			}

			save("./sourcedk/gameconfigurations/" + name + "/game.conf");

			app->PopState();
			return;
		}
		case CALLBACKS::CANCEL:
			app->PopState();
			return;
		};
	}
}

void GameConfigurationEditState::Update()
{
	// HACK: this is a workaround for decorations not being used directly by the app
	// TODO: this might cause problems if we have to hide the window for whatever reason
	if (!app->window->isOpen())
	{
		app->Quit();
		return;
	}
}

void GameConfigurationEditState::Draw()
{
	window->clear(SFUI::Theme::windowBgColor);

	window->draw(*menu);

	window->display();
}

void GameConfigurationEditState::buildHomeInterface(VGUI* interface, SFUI::Menu& menu)
{
	menu.setPosition(sf::Vector2f(10, 10));

	menu.addLabel("Name");
	menu.add(gameName, CALLBACKS::GAME_NAME);

	menu.addLabel("Directory");
	SFUI::HorizontalBoxLayout* hbox1 = menu.addHorizontalBoxLayout();
	hbox1->add(gameDirectory, CALLBACKS::GAME_DIRECTORY);
	hbox1->add(new DisabledButton("Browse"), CALLBACKS::BROWSE);

	menu.addHorizontalBoxLayout();
	menu.addHorizontalBoxLayout();

	SFUI::HorizontalBoxLayout* hbox2 = menu.addHorizontalBoxLayout();
	hbox2->addButton("OK", CALLBACKS::OK);
	hbox2->addButton("Cancel", CALLBACKS::CANCEL);
}

void GameConfigurationEditState::save(std::string filename)
{
	SettingsParser parser(filename);

	std::string name = gameName->getText();
	std::string directory = gameDirectory->getText();

	std::cout << "saving name: " << name << std::endl;
	std::cout << "saving directory: " << directory << std::endl;

	parser.set("name", name);
	parser.set("directory", directory);
}

void GameConfigurationEditState::load(std::string filename)
{
	SettingsParser parser(filename);

	parser.get("name", name);
	parser.get("directory", directory);
}