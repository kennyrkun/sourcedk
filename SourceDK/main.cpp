#include "DisabledButton.hpp"
#include "Windows.hpp"
#include "Games.hpp"
#include "Engine.hpp"
#include "VGUI.hpp"

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>
#include <SFML/Graphics.hpp>

enum CALLBACKS
{
	OPEN_HAMMER,
	OPEN_MODEL_VIEWER,
	OPEN_FACE_POSER,
	OPEN_ITEM_TEST,

	OPEN_SDK_DOCUMENTATION,
	OPEN_SDK_DOCUMENTATION_VALVE,
	OPEN_SDK_RELEASE_NOTES,
	OPEN_SDK_RELEASE_NOTES_VALVE,

	CREATE_A_MOD,
	REFRESH_SDK_CONTENT,
	RESET_GAME_CONFIGURATIONS,
	EDIT_GAME_CONFIGURATIONS,

	APPLICATION_SETTINGS,

	QUIT
};

void buildHomeInterface(VGUI* interface, SFUI::Menu& menu)
{
	menu.setPosition(sf::Vector2f(interface->borderWidth + 10, interface->borderWidth + interface->titlebarHeight + 8));

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

	VGUI decorations;
	decorations.createWindow();
	SFUI::Theme::windowBgColor = decorations.colors.windowBackground;

	sf::RectangleShape lowerBorderExtension;
	lowerBorderExtension.setSize(sf::Vector2f(decorations.windowSize.x - (decorations.borderWidth * 2), decorations.BOTTOM));
	lowerBorderExtension.setPosition(sf::Vector2f(decorations.borderWidth, decorations.windowSize.y - decorations.BOTTOM));

	SFUI::Menu menu(*decorations.getWindow());
	menu.setPosition(sf::Vector2f(decorations.borderWidth + 10, decorations.borderWidth + decorations.titlebarHeight + 8));

	buildHomeInterface(&decorations, menu);

	sf::RectangleShape background;
	background.setSize(sf::Vector2f(decorations.getWindow()->getSize().x - decorations.right.getSize().x - decorations.left.getSize().x, 80 - decorations.bottom.getSize().y));
	background.setPosition(sf::Vector2f(decorations.bottomUpperBorder.getPosition().x + 1, decorations.bottomUpperBorder.getPosition().y + 1));
	background.setFillColor(decorations.colors.border);

	while (decorations.getWindow()->isOpen())
	{
		bool mouseOverCloseButton = false;

		sf::Event event;
		while (decorations.getWindow()->pollEvent(event))
		{
			decorations.HandleEvent(event);

			if (event.type == sf::Event::EventType::Closed)
			{
				decorations.getWindow()->close();
			}
			else if (event.type == sf::Event::EventType::Resized)
			{
				background.setSize(sf::Vector2f(event.size.width - (decorations.borderWidth * 2), background.getSize().y));
				background.setPosition(decorations.bottom.getPosition() - sf::Vector2f(-decorations.borderWidth, background.getSize().y));
			}

			int id = menu.onEvent(event);
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

		decorations.getWindow()->clear(SFUI::Theme::windowBgColor);

		decorations.getWindow()->draw(background);

		decorations.getWindow()->draw(menu);

		// should always be the last thing that gets drawn
		decorations.Draw();

		decorations.getWindow()->display();
	}

	return EXIT_SUCCESS;
}
