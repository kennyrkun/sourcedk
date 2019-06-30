#include "ManageGameConfigurationState.hpp"
#include "GameConfigurationEditState.hpp"

#include "DisabledButton.hpp"
#include "Windows.hpp"
#include "Games.hpp"
#include "Engine.hpp"
#include "VGUI.hpp"
#include "MessageBox.hpp"

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void ManageGameConfigurationState::Init(AppEngine* appEngine)
{
	app = appEngine;

	window = new sf::RenderWindow(sf::VideoMode(400, 100), "Current Game Configuration:");

	std::cout << "Initialising ManageGameConfigurationState." << std::endl;

	SFUI::Theme::windowBgColor = app->windowDecorations.colors.windowBackground;

	menu = new SFUI::Menu(*app->window);
	menu->setPosition(sf::Vector2f(app->windowDecorations.sizes.left + 10, app->windowDecorations.sizes.top + app->windowDecorations.sizes.titlebar + 8));

	buildHomeInterface(&app->windowDecorations, *menu);
}

void ManageGameConfigurationState::Cleanup()
{
	std::cout << "Cleaning up ManageGameConfigurationState." << std::endl;

	delete menu;
	delete window;

	std::cout << "ManageGameConfigurationState cleaned up." << std::endl;
}

void ManageGameConfigurationState::Pause()
{
	std::cout << "ManageGameConfigurationState paused." << std::endl;
}

void ManageGameConfigurationState::Resume()
{
	std::cout << "ManageGameConfigurationState resumed." << std::endl;
}

void ManageGameConfigurationState::HandleEvents()
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
			app->currentGameConfiguration = configurationSelector->getSelectedValue();

			app->PopState();
			return;
		}
		case CALLBACKS::ADD:
		{
			app->sharedStateData.clear();
			app->PushState(new GameConfigurationEditState);
			return;
		}
		case CALLBACKS::EDIT:
		{
			app->sharedStateData.clear();
			app->sharedStateData.emplace("gameName", app->currentGameConfiguration);

			app->PushState(new GameConfigurationEditState);
			return;
		}
		case CALLBACKS::REMOVE:
		{
			MessageBox::Options mbo = { "Confirmation", "Are you sure you want to delete this configuration?", { "Yes", "No" } };

			MessageBox mb(mbo);
			mb.runBlocking();

			switch (mb.exitCode) // 0 is yes, 1 is no
			{
			case 1:
				break;
			case 0:
				// TODO: delete the config
				fs::remove_all("./sourcedk/gameconfigurations/" + app->currentGameConfiguration);
				delete menu;
				menu = new SFUI::Menu(*window);
				buildHomeInterface(&app->windowDecorations, *menu);
				break;
			}

			break;
		}
		case CALLBACKS::CANCEL:
			app->PopState();
			return;
		};
	}
}

void ManageGameConfigurationState::Update()
{
	// HACK: this is a workaround for decorations not being used directly by the app
	// TODO: this might cause problems if we have to hide the window for whatever reason
	if (!app->window->isOpen())
	{
		app->Quit();
		return;
	}
}

void ManageGameConfigurationState::Draw()
{
	window->clear(SFUI::Theme::windowBgColor);

	window->draw(*menu);

	window->display();
}

void ManageGameConfigurationState::buildHomeInterface(VGUI* interface, SFUI::Menu& menu)
{
	menu.setPosition(sf::Vector2f(10, 10));

	SFUI::HorizontalBoxLayout* hbox1 = menu.addHorizontalBoxLayout();

	configurationSelector = new SFUI::OptionsBox<std::string>;

	auto gameList = app->get_directories("./sourcedk/gameconfigurations/");

	for (size_t i = 0; i < gameList.size(); i++)
	{
		if (gameList[i] != app->currentGameConfiguration)
			configurationSelector->addItem(gameList[i], gameList[i]);
	}

	configurationSelector->addItem(app->currentGameConfiguration, app->currentGameConfiguration);

	hbox1->add(configurationSelector, CALLBACKS::GAME_CONFIGURATION_CHANGE);

	SFUI::VerticalBoxLayout* vbox = hbox1->addVerticalBoxLayout();

	vbox->addButton("OK", CALLBACKS::OK);
	vbox->addButton("Cancel", CALLBACKS::CANCEL);

	SFUI::HorizontalBoxLayout* hbox2 = menu.addHorizontalBoxLayout();

	hbox2->addButton("Edit...", CALLBACKS::EDIT);
	hbox2->addButton("Add", CALLBACKS::ADD);
	hbox2->addButton("Remove", CALLBACKS::REMOVE);
}