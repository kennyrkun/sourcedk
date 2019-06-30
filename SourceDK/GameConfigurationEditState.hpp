#ifndef GAME_CONFIGURATION_EDIT_STATE_HPP
#define GAME_CONFIGURATION_EDIT_STATE_HPP

#include "AppState.hpp"

#include "VGUI.hpp"

#include <SFUI/SFUI.hpp>

class GameConfigurationEditState : public AppState
{
public:
	void Init(AppEngine* app);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

private:
	AppEngine* app;

	sf::RenderWindow* window;

	SFUI::Menu* menu;

	std::string oldName, name, directory;

	SFUI::InputBox* gameName, *gameDirectory;

	void buildHomeInterface(VGUI* interface, SFUI::Menu& menu);

	void save(std::string filename);
	void load(std::string filename);

	SFUI::OptionsBox<std::string>* configurationSelector;

	// TODO: make these follow the normal style for Enums
	enum CALLBACKS
	{
		GAME_NAME,
		GAME_DIRECTORY,
		BROWSE,
		OK,
		CANCEL
	};

};

#endif // !GAME_CONFIGURATION_CREATION_STATE_HPP