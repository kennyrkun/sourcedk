#ifndef VGUI_HPP
#define VGUI_HPP

#include "Windows.hpp"
#include "TitleBarButtons.hpp"

#include <SFUI/SFUI.hpp>
#include <SFML/Graphics.hpp>

#include <string>

class VGUI
{
public:
	VGUI()
	{
		// nothing
	}

	~VGUI();

	sf::RenderWindow* createWindow(const sf::Vector2f& size = { 310, 500 }, const std::string& title_ = "Source SDK");

	sf::RenderWindow* bindToWindow(sf::RenderWindow* window);

	sf::RenderWindow* getWindow() { return window; }

	struct BorderSizes
	{
		int top = 8;
		int topOutline = 1;

		int left = 8;
		int leftOutline = 1;

		int right = 8;
		int rightOutline = 1;

		int bottom = 8;
		int bottomOutline = 1;

		const int titlebar = 31 - top;
	} sizes;

	sf::Text title;

	sf::Vector2f windowSize;
	sf::Vector2f useableWindowSize;
	sf::Vector2u lastWindowSize;
	sf::Vector2u lastWindowSize2;

	sf::Vector2i grabbedOffset;
	bool grabbedWindow = false;
	bool resizingXLeft = false;
	bool resizingXRight = false;
	bool resizingYTop = false;
	bool resizingYBottom = false;
	bool resizing = false;

	sf::Vector2i lastWindowPosition;
	int dragOffsetY;
	int dragOffsetX;

	sf::RectangleShape titlebar;
	sf::RectangleShape top;
	sf::RectangleShape topUpper;
	sf::RectangleShape topLower;
	sf::RectangleShape left;
	sf::RectangleShape leftLeftBorder;
	sf::RectangleShape leftRightBorder;
	sf::RectangleShape right;
	sf::RectangleShape rightLeftBorder;
	sf::RectangleShape rightRightBorder;
	sf::RectangleShape bottom;
	sf::RectangleShape bottomUpperBorder;
	sf::RectangleShape bottomLowerBorder;

	sf::RectangleShape resizer;

	struct Colors
	{
		sf::Color borderWhite = sf::Color(136, 145, 128);
		sf::Color borderBlack = sf::Color(40, 46, 34);
		sf::Color border = sf::Color(76, 88, 68);

		sf::Color windowBackground = sf::Color(62, 70, 55);
	} colors;

	sf::Texture resizerTexture;

	bool isMaximised = false;

	void HandleEvent(const sf::Event& event);

	void Draw();

	void updateWindowDecorations();

private:
	sf::RenderWindow* window;

	TitleBarButtons titleBarButton;

	void initialise();
};

#endif // !VGUI_HPP
