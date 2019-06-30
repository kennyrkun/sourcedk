#ifndef MANAGE_GAME_CONFIGURATION_STATE_HPP
#define MANAGE_GAME_CONFIGURATION_STATE_HPP

#include "AppState.hpp"

#include "VGUI.hpp"

#include <SFUI/Layouts/Menu.hpp>
#include <SFUI/OptionsBox.hpp>

class ManageGameConfigurationState : public AppState
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

	void buildHomeInterface(VGUI* interface, SFUI::Menu& menu);

	SFUI::OptionsBox<std::string>* configurationSelector;

	// TODO: make these follow the normal style for Enums
	enum CALLBACKS
	{
		EDIT = -9999,
		ADD,
		REMOVE,
		OK,
		GAME_CONFIGURATION_CHANGE,
		CANCEL
	};

};

#endif // !MANAGE_GAME_CONFIGURATION_STATE_HPP