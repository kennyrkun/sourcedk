#ifndef APP_ENGINE_HPP
#define APP_ENGINE_HPP

#include "AppSettings.hpp"
#include "VGUI.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <thread>

class AppState;

class AppEngine
{
public:
	void Init(std::string title, AppSettings settings_);
	void Cleanup();

	// THE ACTIVE STATE IS NOT SWITCHED UNTIL THE CALLER FUNCTION HAS RETURNED
	void ChangeState(AppState* state);
	// THE ACTIVE STATE IS NOT SWITCHED UNTIL THE CALLER FUNCTION HAS RETURNED
	void PushState(AppState* state);
	// THE ACTIVE STATE IS NOT SWITCHED UNTIL THE CALLER FUNCTION HAS RETURNED
	void PopState();

	AppState* GetCurrentState() { return states.back(); };

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return running; }

	// THE ACTIVE STATE IS NOT SWITCHED UNTIL THE CALLER FUNCTION HAS RETURNED
	void Quit();

	bool multithreaded_process_running;
	bool multithreaded_process_finished;
	void ShowMultiThreadedIndicator();
	void SetMultiThreadedIndicatorPosition(const sf::Vector2f& pos);
	void SetMultiThreadedIndicatorIcon(sf::Texture* texture);
	std::thread *multithread;
	/// should be deleted by Update method of State in which it is used.

	const std::string currentDateTime();

	sf::RenderWindow* window;
	AppSettings settings;

	VGUI windowDecorations;

private:
	// the stack of states
	std::vector<AppState*> states;

	enum class EventType
	{
		PushState,
		ChangeState,
		PopState,
		Quit
	};

	std::vector<std::pair<EventType, AppState*>> queuedEvents;

	bool running;

	sf::CircleShape multithreaded_process_indicator;
	sf::Texture multithreaded_process_indicator_tex;
};

#endif // !APP_ENGINE_HPP
