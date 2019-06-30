#include "MainMenuState.hpp"
#include "ManageGameConfigurationState.hpp"

#include "DisabledButton.hpp"
#include "Windows.hpp"
#include "Games.hpp"
#include "Engine.hpp"
#include "VGUI.hpp"
#include "SettingsParser.hpp"
#include "MessageBox.hpp"

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void MainMenuState::Init(AppEngine* appEngine)
{
	app = appEngine;

	std::cout << "Initialising MainMenuState." << std::endl;

	SFUI::Theme::windowBgColor = app->windowDecorations.colors.windowBackground;

	menu = new SFUI::Menu(*app->window);
	menu->setPosition(sf::Vector2f(app->windowDecorations.sizes.left + 10, app->windowDecorations.sizes.top + app->windowDecorations.sizes.titlebar + 8));

	buildHomeInterface(&app->windowDecorations, *menu);
}

void MainMenuState::Cleanup()
{
	std::cout << "Cleaning up MainMenuState." << std::endl;

	delete menu;

	std::cout << "MainMenuState cleaned up." << std::endl;
}

void MainMenuState::Pause()
{
	std::cout << "MainMenuState paused." << std::endl;
}

void MainMenuState::Resume()
{
	if (app->currentGameConfiguration != gameConfiguration->getSelectedValue())
	{
		std::cout << "game configuration has changed, reloading menu" << std::endl;

		delete menu;

		menu = new SFUI::Menu(*app->window);
		menu->setPosition(sf::Vector2f(app->windowDecorations.sizes.left + 10, app->windowDecorations.sizes.top + app->windowDecorations.sizes.titlebar + 8));

		buildHomeInterface(&app->windowDecorations, *menu);
	}

	std::cout << "MainMenuState resumed." << std::endl;
}

void MainMenuState::HandleEvents()
{
	sf::Event event;
	while (app->window->pollEvent(event))
	{
		app->windowDecorations.HandleEvents(event);

		if (event.type == sf::Event::EventType::Closed)
			app->windowDecorations.getWindow()->close();

		int id = menu->onEvent(event);
		switch (id)
		{
		case CALLBACKS::RESET_GAME_CONFIGURATIONS:
		{
			MessageBox::Options mbo = { "Confirmation", "Are you sure you want to reset all configurations?\n\nYou will have to manually recreate them all.", { "Yes", "No" } };

			MessageBox mb(mbo);
			mb.runBlocking();

			switch (mb.exitCode) // 0 is yes, 1 is no
			{
			case 1:
				break;
			case 0:
				try
				{
					fs::remove_all("./sourcedk/gameconfigurations/");
				}
				catch (std::exception &e)
				{
					std::cerr << "failed to delete all configurations" << std::endl;
					std::cerr << e.what() << std::endl;
				}

				delete menu;

				menu = new SFUI::Menu(*app->window);
				menu->setPosition(sf::Vector2f(app->windowDecorations.sizes.left + 10, app->windowDecorations.sizes.top + app->windowDecorations.sizes.titlebar + 8));

				buildHomeInterface(&app->windowDecorations, *menu);
			}

			break;
		}
		case CALLBACKS::GAME_CONFIGURATION_CHANGE:
		{
			app->currentGameConfiguration = gameConfiguration->getSelectedValue();
			break;
		}
		case CALLBACKS::EDIT_GAME_CONFIGURATIONS:
			app->PushState(new ManageGameConfigurationState);
			return;
		case CALLBACKS::OPEN_SDK_RELEASE_NOTES:
			system("start https://github.com/kennyrkun/sourcedk/releases");
			break;
		case CALLBACKS::OPEN_SDK_RELEASE_NOTES_VALVE:
			system("start https://developer.valvesoftware.com/wiki/Source_SDK_Release_Notes");
			break;
		case CALLBACKS::OPEN_SDK_DOCUMENTATION:
			system("start https://github.com/kennyrkun/sourcedk/wiki");
			break;
		case CALLBACKS::OPEN_SDK_DOCUMENTATION_VALVE:
			system("start https://developer.valvesoftware.com/wiki/Main_Page");
			break;
		};
	}
}

void MainMenuState::Update()
{
	// HACK: this is a workaround for decorations not being used directly by the app
	// TODO: this might cause problems if we have to hide the window for whatever reason
	if (!app->window->isOpen())
	{
		app->Quit();
		return;
	}
}

void MainMenuState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);

	app->window->draw(*menu);

	app->windowDecorations.Draw();
	app->window->display();
}

std::string getGameDirectory(std::string name)
{
	SettingsParser parser("./sourcedk/gameconfigurations/" + name + "/game.conf");

	std::string directory;

	parser.get("directory", directory);

	return directory;
}

void MainMenuState::buildHomeInterface(VGUI* interface, SFUI::Menu& menu)
{
	menu.setPosition(sf::Vector2f(interface->sizes.left + 10, interface->sizes.top + interface->sizes.titlebar + 8));

	// TODO: allow this to be customised with settings
	menu.addLabel("Applications");

	/*
	if (fs::exists(getGameDirectory(app->currentGameConfiguration) + "/bin/Hammer.exe"))
		menu.addButton("Hammer Editor", OPEN_HAMMER);
	else
	*/
		menu.add(new DisabledButton("Hammer Editor"), OPEN_HAMMER);

	menu.add(new DisabledButton("Model Viewer"), OPEN_MODEL_VIEWER);
	menu.add(new DisabledButton("Face Poser"), OPEN_FACE_POSER);
	menu.add(new DisabledButton("itemtest"), OPEN_ITEM_TEST);

	menu.addLabel("Documentation");
	SFUI::HorizontalBoxLayout* hbox1 = menu.addHorizontalBoxLayout();
	hbox1->addButton("SDK Release Notes", OPEN_SDK_RELEASE_NOTES);
	hbox1->addButton("Valve", OPEN_SDK_RELEASE_NOTES_VALVE);
	SFUI::HorizontalBoxLayout* hbox2 = menu.addHorizontalBoxLayout();
	hbox2->addButton("SDK Documentation", OPEN_SDK_DOCUMENTATION);
	hbox2->addButton("Valve", OPEN_SDK_DOCUMENTATION_VALVE);

	menu.addLabel("Utilities");
	menu.add(new DisabledButton("Create a Mod"), CREATE_A_MOD);
	menu.add(new DisabledButton("Refresh SDK Content"), REFRESH_SDK_CONTENT);
	menu.addButton("Reset Game Configurations", RESET_GAME_CONFIGURATIONS);
	menu.addButton("Manage Game Configurations", EDIT_GAME_CONFIGURATIONS);

	menu.addHorizontalBoxLayout();
	menu.add(new DisabledButton("Application Settings"), APPLICATION_SETTINGS);

	// padding
	menu.addHorizontalBoxLayout();
	menu.addHorizontalBoxLayout();

	menu.addLabel("Current Game Configuration:");

	//	SFUI::ComboBox<Game>* gameCombo = new SFUI::ComboBox<Game>(&window);
	gameConfiguration = new SFUI::OptionsBox<std::string>;

	auto gameList = app->get_directories("./sourcedk/gameconfigurations/");

	for (size_t i = 0; i < gameList.size(); i++)
	{
		if (gameList[i] != app->currentGameConfiguration)
			gameConfiguration->addItem(gameList[i], gameList[i]);
	}

	gameConfiguration->addItem(app->currentGameConfiguration, app->currentGameConfiguration);

	menu.add(gameConfiguration, CALLBACKS::GAME_CONFIGURATION_CHANGE);
}