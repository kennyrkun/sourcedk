#ifndef VGUI_HPP
#define VGUI_HPP

#include "Windows.hpp"

#include <SFUI/SFUI.hpp>
#include <SFML/Graphics.hpp>

#include <string>

class VGUI
{
	~VGUI()
	{
		delete closeMenu;
	}

	sf::RenderWindow& createWindow(sf::Vector2f size = { 310, 500 }, std::string title_ = "Source SDK")
	{
		windowSize = size;
		useableWindowSize = { windowSize.x - (borderWidth * 2), windowSize.y - (borderWidth * 2) - titlebarHeight };

		window.create(sf::VideoMode(windowSize.x, windowSize.y), title.getString(), sf::Style::None);
		window.setVerticalSyncEnabled(true);

		closeMenu = new SFUI::Menu(window);

		SFUI::HorizontalBoxLayout* closehbox = closeMenu->addHorizontalBoxLayout();

		minimiseButtonTexture.loadFromFile("resources/interface/minimise.png");
		minimiseButton = new SFUI::SpriteButton(minimiseButtonTexture);
		closehbox->add(minimiseButton, DECO_CALLBACK::MINIMISED);

		/*
		// does this application really need to be maximised?
		maximiseButtonTexture.loadFromFile("resources/interface/maximise.png");
		maximiseButton = new SFUI::SpriteButton(maximiseButtonTexture);
		closehbox->add(maximiseButton, DECO_CALLBACK::MAXIMISED);
		*/

		closeButtonTexture.loadFromFile("resources/interface/close.png");
		closeButton = new SFUI::SpriteButton(closeButtonTexture);
		closehbox->add(closeButton, DECO_CALLBACK::CLOSED);

		closeMenu->setPosition(sf::Vector2f(windowSize.x - closeMenu->getSize().x - borderWidth, borderWidth - 2));

		titlebar.setFillColor(colors.BORDER);
		top.setFillColor(colors.BORDER);
		topUpper.setFillColor(colors.WHITE_BORDER);
		topLower.setFillColor(colors.BLACK_BORDER);
		left.setFillColor(colors.BORDER);
		leftLeftBorder.setFillColor(colors.WHITE_BORDER);
		leftRightBorder.setFillColor(colors.BLACK_BORDER);
		right.setFillColor(colors.BORDER);
		rightLeftBorder.setFillColor(colors.WHITE_BORDER);
		rightRightBorder.setFillColor(colors.BLACK_BORDER);
		bottom.setFillColor(colors.BORDER);
		bottomUpperBorder.setFillColor(colors.WHITE_BORDER);
		bottomLowerBorder.setFillColor(colors.BLACK_BORDER);

		resizerTexture.loadFromFile("resources/interface/resizer.png");
		resizer.setSize(sf::Vector2f(resizerTexture.getSize().x, resizerTexture.getSize().x));
		resizer.setTexture(&resizerTexture);

		resizer.setPosition(sf::Vector2f(window.getSize() - sf::Vector2u(borderWidth, borderWidth)));

		title.setString(title_);
		title.setFont(SFUI::Theme::getFont());
		title.setCharacterSize(16);

		updateWindowDecorations();

		return window;
	}

	const int borderWidth = 8;
	const int titlebarHeight = (31 - borderWidth);
	const int BOTTOM = 80;

	sf::Text title;
	sf::Vector2f windowSize;
	sf::Vector2f useableWindowSize;

	sf::Vector2i grabbedOffset;
	bool grabbedWindow = false;
	bool resizingXLeft = false;
	bool resizingXRight = false;
	bool resizingYTop = false;
	bool resizingYBottom = false;
	bool resizing = false;

	sf::Vector2u lastWindowSize;
	sf::Vector2u lastWindowSize2;
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

	SFUI::Menu* closeMenu;

	SFUI::SpriteButton* minimiseButton;
	SFUI::SpriteButton* maximiseButton;
	SFUI::SpriteButton* closeButton;

	struct Colors
	{
		sf::Color WHITE_BORDER = sf::Color(136, 145, 128);
		sf::Color BLACK_BORDER = sf::Color(40, 46, 34);

		sf::Color WINDOW_BACKGROUND = sf::Color(62, 70, 55);
		sf::Color BORDER = sf::Color(76, 88, 68);
	} colors;

	sf::Texture minimiseButtonTexture;
	sf::Texture maximiseButtonTexture;
	sf::Texture closeButtonTexture;

	sf::Texture resizerTexture;

	bool isMaximised = false;

	void HandleEvent(sf::Event& event)
	{
		// TODO: don't run all the resizing code if the window is not resizable

		if (event.type == sf::Event::EventType::Resized)
		{
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			window.setView(sf::View(visibleArea));

			updateWindowDecorations();

			closeMenu->setPosition(sf::Vector2f(windowSize.x - closeMenu->getSize().x - borderWidth, borderWidth - 2));
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				// TODO: don't drag if over closeMenu
				if (titlebar.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					grabbedWindow = true;

				if (top.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					resizingYTop = true;
					resizing = true;
				}
				if (left.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					resizingXLeft = true;
					resizing = true;
				}
				if (right.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					resizingXRight = true;
					resizing = true;
				}
				if (bottom.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					resizingYBottom = true;
					resizing = true;
				}

				grabbedOffset = window.getPosition() - sf::Mouse::getPosition();
				dragOffsetX = sf::Mouse::getPosition().x;
				dragOffsetY = sf::Mouse::getPosition().y;

				lastWindowSize = window.getSize();
				lastWindowPosition = window.getPosition();
			}
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				// FIXME: fix this horseshit
				// only run this here so it only triggers if the mouse is released
				if (!grabbedWindow)
				{
					int id = closeMenu->onEvent(event);
					switch (id)
					{
					case DECO_CALLBACK::CLOSED:
					{
						window.close();
						break;
					}
					};
				}

				grabbedWindow = false;
				resizingXLeft = false;
				resizingXRight = false;
				resizingYTop = false;
				resizingYBottom = false;
				resizing = false;
			}
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			// TODO: optimise this so it's not dog shit
			// TODO: check if mouse is in regular bounds first

			// FIXME: SFML cursor is invisible through discord screenshare.
			// perhaps this is due to the usage of CopyCursor rather than SetCursor in the SFML source.

			if (top.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
			{
				sf::Cursor cursor;

				if (left.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					cursor.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);
				else if (right.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					cursor.loadFromSystem(sf::Cursor::SizeBottomLeftTopRight);
				else
					cursor.loadFromSystem(sf::Cursor::SizeVertical);

				window.setMouseCursor(cursor);
			}
			else if (left.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
			{
				sf::Cursor cursor;

				if (top.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					cursor.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);
				else if (bottom.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					cursor.loadFromSystem(sf::Cursor::SizeBottomLeftTopRight);
				else
					cursor.loadFromSystem(sf::Cursor::SizeHorizontal);

				window.setMouseCursor(cursor);
			}
			else if (right.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
			{
				sf::Cursor cursor;

				if (bottom.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					cursor.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);
				else if (top.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					cursor.loadFromSystem(sf::Cursor::SizeBottomLeftTopRight);
				else
					cursor.loadFromSystem(sf::Cursor::SizeHorizontal);

				window.setMouseCursor(cursor);
			}
			else if (bottom.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
			{
				sf::Cursor cursor;

				if (left.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					cursor.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);
				else if (right.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					cursor.loadFromSystem(sf::Cursor::SizeBottomLeftTopRight);
				else
					cursor.loadFromSystem(sf::Cursor::SizeVertical);

				window.setMouseCursor(cursor);
			}
			else
			{
				sf::Cursor cursor;
				if (cursor.loadFromSystem(sf::Cursor::Arrow))
					window.setMouseCursor(cursor);
			}

			if (resizing)
			{
				sf::Vector2u newWindowSize(lastWindowSize);
				sf::Vector2i newWindowPosition(lastWindowPosition);

				if (resizingXRight)
					newWindowSize.x = lastWindowSize.x + sf::Mouse::getPosition().x - dragOffsetX;

				if (resizingYBottom)
					newWindowSize.y = sf::Mouse::getPosition().y + lastWindowSize.y - dragOffsetY;

				if (resizingYTop)
				{
					newWindowSize.y = lastWindowSize.y - sf::Mouse::getPosition().y + dragOffsetY;
					newWindowPosition.y = lastWindowPosition.y - (window.getSize().y - lastWindowSize.y);
				}

				if (resizingXLeft)
				{
					newWindowSize.x = lastWindowSize.x - sf::Mouse::getPosition().x + dragOffsetX;
					newWindowPosition.x = lastWindowPosition.x - (window.getSize().x - lastWindowSize.x);
				}

				window.setSize(newWindowSize);
				windowSize = sf::Vector2f(window.getSize());

				window.setPosition(newWindowPosition);
			}
		}

		int id = closeMenu->onEvent(event);
		switch (id)
		{
		case DECO_CALLBACK::CLOSED:
			window.close();
			/*
			sf::FloatRect clsbtn(closeButton->getAbsolutePosition(), closeButton->getSize());
			if (clsbtn.contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
			{
			}
			else
			{
				// TODO: make this not suck
				closeButton->onStateChanged(SFUI::State::Default);
			}
			*/
			break;
		case DECO_CALLBACK::MAXIMISED:
		{
			if (!isMaximised)
			{
				windows::window::maximise(window.getSystemHandle());
				maximiseButtonTexture.loadFromFile("resources/interface/unmaximise.png");
			}
			else
			{
				windows::window::normal(window.getSystemHandle());
				maximiseButtonTexture.loadFromFile("resources/interface/maximise.png");
			}

			isMaximised = !isMaximised;
			break;
		}
		case DECO_CALLBACK::MINIMISED:
			windows::window::minimise(window.getSystemHandle());
			break;
		};

		if (grabbedWindow)
			window.setPosition(sf::Mouse::getPosition() + grabbedOffset);
	}

	void Draw()
	{
		window.draw(titlebar);
		window.draw(top);
		window.draw(title);
		window.draw(left);
		window.draw(right);

		window.draw(topLower);
		window.draw(topUpper);
		window.draw(rightLeftBorder);
		window.draw(leftRightBorder);

		window.draw(bottom);
		window.draw(leftLeftBorder);
		window.draw(rightRightBorder);

		window.draw(bottomUpperBorder);
		window.draw(bottomLowerBorder);

		window.draw(resizer);

		window.draw(*closeMenu);
	}

	void updateWindowDecorations()
	{
		title.setPosition(sf::Vector2f(borderWidth, (borderWidth / 2)));

		titlebar.setSize(sf::Vector2f(windowSize.x - borderWidth * 2, titlebarHeight));
		titlebar.setPosition(sf::Vector2f(borderWidth, borderWidth));

		top.setSize(sf::Vector2f(windowSize.x, borderWidth));

		topUpper.setSize(sf::Vector2f(top.getSize().x, 1));

		topLower.setSize(sf::Vector2f(windowSize.x - (borderWidth * 2), 1));
		topLower.setPosition(sf::Vector2f(borderWidth, titlebarHeight + borderWidth - 1));

		left.setSize(sf::Vector2f(borderWidth, windowSize.y));

		leftLeftBorder.setSize(sf::Vector2f(1, left.getSize().y));

		leftRightBorder.setSize(sf::Vector2f(1, left.getSize().y - BOTTOM - titlebarHeight - borderWidth + 1));
		leftRightBorder.setPosition(sf::Vector2f(borderWidth - 1, titlebarHeight + borderWidth - 1));

		right.setSize(sf::Vector2f(borderWidth, windowSize.y));
		right.setPosition(sf::Vector2f(windowSize.x - borderWidth, 0));

		rightLeftBorder.setSize(sf::Vector2f(1, right.getSize().y - BOTTOM - titlebarHeight - borderWidth + 1));
		rightLeftBorder.setPosition(sf::Vector2f(windowSize.x - borderWidth, titlebarHeight + borderWidth - 1));

		rightRightBorder.setSize(sf::Vector2f(1, right.getSize().y));
		rightRightBorder.setPosition(sf::Vector2f(windowSize.x - 1, 0));

		bottom.setSize(sf::Vector2f(windowSize.x + (borderWidth * 2), borderWidth));
		bottom.setPosition(sf::Vector2f(0, windowSize.y - borderWidth));

		bottomUpperBorder.setSize(sf::Vector2f(windowSize.x - (borderWidth * 2) + 2, 1));
		bottomUpperBorder.setPosition(sf::Vector2f(borderWidth - 1, windowSize.y - BOTTOM));

		bottomLowerBorder.setSize(sf::Vector2f(windowSize.x + (borderWidth * 2), 1));
		bottomLowerBorder.setPosition(sf::Vector2f(0, windowSize.y - 1));

		resizer.setPosition(sf::Vector2f(window.getSize().x - resizer.getSize().y - 4, window.getSize().y - resizer.getSize().y - 4));
	}

private:
	sf::RenderWindow window;

	enum DECO_CALLBACK
	{
		CLOSED,
		MINIMISED,
		MAXIMISED
	};
};

#endif // !VGUI_HPP
