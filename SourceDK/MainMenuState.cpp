#include "MainMenuState.hpp"

#include "DisabledButton.hpp"
#include "Windows.hpp"
#include "Games.hpp"
#include "Engine.hpp"
#include "VGUI.hpp"

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

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
	std::cout << "MainMenuState cleaned up." << std::endl;
}

void MainMenuState::Pause()
{
	std::cout << "MainMenuState paused." << std::endl;
}

void MainMenuState::Resume()
{
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

void MainMenuState::buildHomeInterface(VGUI* interface, SFUI::Menu& menu)
{
	menu.setPosition(sf::Vector2f(interface->sizes.left + 10, interface->sizes.top + interface->sizes.titlebar + 8));

	// TODO: allow this to be customised with settings
	menu.addLabel("Applications");
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
	menu.add(new DisabledButton("Reset Game Configurations"), RESET_GAME_CONFIGURATIONS);
	menu.add(new DisabledButton("Edit Game Configurations"), EDIT_GAME_CONFIGURATIONS);

	menu.addHorizontalBoxLayout();
	menu.add(new DisabledButton("Application Settings"), APPLICATION_SETTINGS);

	// padding
	menu.addHorizontalBoxLayout();
	menu.addHorizontalBoxLayout();
	menu.addHorizontalBoxLayout();

	SFUI::FormLayout* form2 = menu.addFormLayout();

	//	SFUI::ComboBox<Engine>* engineVersion = new SFUI::ComboBox<Engine>(&window);
		// We can't use ComboBox right now because SFUI isn't very great.
	SFUI::OptionsBox<Engine>* engineVersion = new SFUI::OptionsBox<Engine>;
	engineVersion->addItem("GoldSrc", Engine::GoldSource);
	engineVersion->addItem("2003", Engine::Source2003);
	engineVersion->addItem("2006", Engine::Source2006);
	engineVersion->addItem("2007", Engine::Source2007);
	engineVersion->addItem("2009", Engine::Source2009);
	engineVersion->addItem("2013", Engine::Source2013);
	engineVersion->addItem("Source2", Engine::Source2);
	form2->addRow("Engine: ", engineVersion, 1336);

	//	SFUI::ComboBox<Game>* gameCombo = new SFUI::ComboBox<Game>(&window);
	SFUI::OptionsBox<Game>* gameCombo = new SFUI::OptionsBox<Game>;
	gameCombo->addItem("Counter-Strike: Global Offensive", Game::CSGO);
	gameCombo->addItem("Counter-Strike: Source", Game::CSS);
	gameCombo->addItem("Counter-Strike", Game::CS);
	gameCombo->addItem("Half-Life", Game::HL);
	gameCombo->addItem("Half-Life: Source", Game::HLS);
	gameCombo->addItem("Half-Life: Opposing Force", Game::HLOS);
	gameCombo->addItem("Half-Life: Blue Shift", Game::HLBS);
	gameCombo->addItem("Half-Life: Deathmatch", Game::HLDM);
	gameCombo->addItem("Half-Life: Deathmatch: Source", Game::HLDMS);
	gameCombo->addItem("Half-Life 2", Game::HL2);
	gameCombo->addItem("Half-Life 2: Episode 1", Game::HL2E1);
	gameCombo->addItem("Half-Life 2: Episode 2", Game::HL2E2);
	gameCombo->addItem("Half-Life 2: Lost Coast", Game::HL2LC);
	gameCombo->addItem("Half-Life 2: Deathmatch", Game::HL2DM);
	gameCombo->addItem("Dota 2", Game::DOTA2);
	gameCombo->addItem("Team Fortress 2", Game::TF2);
	gameCombo->addItem("Left 4 Dead", Game::L4D);
	gameCombo->addItem("Left 4 Dead 2", Game::L4D2);
	gameCombo->addItem("Ricochet", Game::RICOCHET);
	form2->addRow("Game: ", gameCombo, 1337);
}