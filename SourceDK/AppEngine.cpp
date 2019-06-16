#include "AppEngine.hpp"
#include "AppState.hpp"

#include <SFUI/Theme.hpp>
#include <iostream>
#include <ctime>

// TODO: load program icon

void AppEngine::Init(std::string title_, AppSettings settings_)
{
	std::cout << "AppEngine Init" << std::endl;

	settings = settings_;

	window = new sf::RenderWindow(sf::VideoMode(settings.window.width, settings.window.height), title_, sf::Style::None);

	windowDecorations.bindToWindow(window, title_);
	window->setVerticalSyncEnabled(settings.window.verticalSync);

	multithreaded_process_indicator.setRadius(20);
	multithreaded_process_indicator.setOrigin(sf::Vector2f(20, 20));
//	SetMultiThreadedIndicatorIcon(GBL::theme.getTexture("settings_2x.png")); // eventually this needs to be done in IntialiseState, but right now it refuses to work there.

	running = true;
}

void AppEngine::Cleanup()
{
	std::cout << "Cleaning up AppEngine." << std::endl;
	
	for (size_t i = 0; i < states.size(); i++)
		PopState();

	window->close();
	delete window;

	running = false;

	std::cout << "AppEngine cleaned up." << std::endl;
}

void AppEngine::ChangeState(AppState* state)
{
	queuedEvents.push_back(std::pair<EventType, AppState*>(EventType::ChangeState, state));
}

void AppEngine::PushState(AppState* state)
{
	queuedEvents.push_back(std::pair<EventType, AppState*>(EventType::PushState, state));
}

void AppEngine::PopState()
{
	queuedEvents.push_back(std::pair<EventType, AppState*>(EventType::PopState, nullptr));
}

void AppEngine::HandleEvents()
{
	if (running && !states.empty())
		states.back()->HandleEvents();

	for (size_t i = 0; i < queuedEvents.size(); i++)
	{
		if (queuedEvents[i].first == EventType::ChangeState)
		{
			if (!states.empty())
			{
				states.back()->Cleanup();

				delete states.back();
				states.pop_back();
			}

			states.push_back(queuedEvents[i].second);
			states.back()->Init(this);
		}
		else if (queuedEvents[i].first == EventType::PushState)
		{
			if (!states.empty())
				states.back()->Pause();

			states.push_back(queuedEvents[i].second);
			states.back()->Init(this);
		}
		else if (queuedEvents[i].first == EventType::PopState)
		{
			if (!states.empty())
			{
				states.back()->Cleanup();

				delete states.back();
				states.pop_back();
			}

			if (!states.empty())
				states.back()->Resume();
		}
		else if (queuedEvents[i].first == EventType::Quit)
		{
			Quit();
		}

		// this might break things, but I'm not sure.
		// I am confident it will work.
		queuedEvents.pop_back();
	}
}

void AppEngine::Update()
{
	if (running)
		states.back()->Update();
}

void AppEngine::Draw()
{
	if (running)
		states.back()->Draw();
}

void AppEngine::ShowMultiThreadedIndicator()
{
	multithreaded_process_indicator.rotate(1);
	window->draw(multithreaded_process_indicator);
}

void AppEngine::SetMultiThreadedIndicatorPosition(const sf::Vector2f& pos)
{
	multithreaded_process_indicator.setPosition(pos);
}

void AppEngine::SetMultiThreadedIndicatorIcon(sf::Texture* texture)
{
	multithreaded_process_indicator.setTexture(texture);
}

// https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c/10467633#10467633
const std::string AppEngine::currentDateTime() 
{
	time_t     now = time(0);
	struct tm  timeinfo;
	char       buf[80];
	localtime_s(&timeinfo, &now);
	strftime(buf, sizeof(buf), "%F.%H-%M-%S", &timeinfo);

	return buf;
}

void AppEngine::Quit()
{
	for (size_t i = 0; i < states.size(); i++)
	{
		states.back()->Cleanup();
		delete states.back();
		states.pop_back();
	}

	states.clear();

	running = false;
}
