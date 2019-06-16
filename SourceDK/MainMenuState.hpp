#ifndef MAIN_MENU_STATE_HPP
#define MAIN_MENU_STATE_HPP

#include "AppState.hpp"

#include "VGUI.hpp"
#include <SFUI/Layouts/Menu.hpp>

class MainMenuState : public AppState
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

	SFUI::Menu* menu;

	void buildHomeInterface(VGUI* interface, SFUI::Menu& menu);

	// TODO: make these follow the normal style for Enums
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
};

#endif // !MAIN_MENU_STATE_HPP