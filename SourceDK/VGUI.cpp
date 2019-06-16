#include "VGUI.hpp"

VGUI::~VGUI()
{
	delete window;
//	delete titleBarButtons;
}

sf::RenderWindow* VGUI::createWindow(const sf::Vector2f& size, const std::string& title_)
{
	windowSize = size;
	useableWindowSize = { windowSize.x - (sizes.left + sizes.right), windowSize.y - (sizes.top + sizes.bottom) - sizes.titlebar };

	window = new sf::RenderWindow(sf::VideoMode(size.x, size.y), title_, sf::Style::None);
	window->setVerticalSyncEnabled(true);

	title.setString(title_);

	initialise();

	return window;
}

sf::RenderWindow* VGUI::bindToWindow(sf::RenderWindow* window)
{
	this->window = window;

	initialise();

	return this->window;
}

void VGUI::HandleEvent(const sf::Event& event)
{
	// TODO: don't run all the resizing code if the window is not resizable\

	sf::Vector2i mousePosition = sf::Mouse::getPosition();
	sf::Vector2i mousePositionWindow = sf::Mouse::getPosition(*window);

	if (event.type == sf::Event::EventType::Resized)
	{
		sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
		window->setView(sf::View(visibleArea));

		updateWindowDecorations();

		titleBarButton.closeMenu->setPosition(sf::Vector2f(windowSize.x - titleBarButton.closeMenu->getSize().x - sizes.right, sizes.top - 2));
		titleBarButton.menuBounds = { titleBarButton.closeMenu->getAbsolutePosition(), titleBarButton.closeMenu->getSize() };
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (titleBarButton.menuBounds.contains(window->mapPixelToCoords(mousePositionWindow)))
			{
				std::cout << "pressin a button on the thing" << std::endl;

				titleBarButton.onEvent(event);
			}
			else
			{
				if (titlebar.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
				{
					std::cout << "window grabbed" << std::endl;
					grabbedWindow = true;
				}
			}

			if (top.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
			{
				resizingYTop = true;
				resizing = true;
			}
			if (left.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
			{
				resizingXLeft = true;
				resizing = true;
			}
			if (right.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
			{
				resizingXRight = true;
				resizing = true;
			}
			if (bottom.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
			{
				resizingYBottom = true;
				resizing = true;
			}

			grabbedOffset = window->getPosition() - sf::Mouse::getPosition();
			dragOffsetX = sf::Mouse::getPosition().x;
			dragOffsetY = sf::Mouse::getPosition().y;

			lastWindowSize = window->getSize();
			lastWindowPosition = window->getPosition();
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			// FIXME: fix this horseshit
			// only run this here so it only triggers if the mouse is released
			if (!grabbedWindow)
				titleBarButton.onEvent(event);

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
		// - the cursor is recreated and applied everytime the mouse is moved

		// TODO: check if mouse is in regular bounds first

		// FIXME: SFML cursor is invisible through discord screenshare.
		// perhaps this is due to the usage of CopyCursor rather than SetCursor in the SFML source.

		if (titleBarButton.menuBounds.contains(window->mapPixelToCoords(mousePositionWindow)))
			titleBarButton.onEvent(event);

		if (top.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
		{
			sf::Cursor cursor;

			if (left.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
				cursor.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);
			else if (right.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
				cursor.loadFromSystem(sf::Cursor::SizeBottomLeftTopRight);
			else
				cursor.loadFromSystem(sf::Cursor::SizeVertical);

			window->setMouseCursor(cursor);
		}
		else if (left.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
		{
			sf::Cursor cursor;

			if (top.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
				cursor.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);
			else if (bottom.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
				cursor.loadFromSystem(sf::Cursor::SizeBottomLeftTopRight);
			else
				cursor.loadFromSystem(sf::Cursor::SizeHorizontal);

			window->setMouseCursor(cursor);
		}
		else if (right.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
		{
			sf::Cursor cursor;

			if (bottom.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
				cursor.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);
			else if (top.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
				cursor.loadFromSystem(sf::Cursor::SizeBottomLeftTopRight);
			else
				cursor.loadFromSystem(sf::Cursor::SizeHorizontal);

			window->setMouseCursor(cursor);
		}
		else if (bottom.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
		{
			sf::Cursor cursor;

			if (left.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
				cursor.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);
			else if (right.getGlobalBounds().contains(window->mapPixelToCoords(mousePositionWindow)))
				cursor.loadFromSystem(sf::Cursor::SizeBottomLeftTopRight);
			else
				cursor.loadFromSystem(sf::Cursor::SizeVertical);

			window->setMouseCursor(cursor);
		}
		else
		{
			sf::Cursor cursor;
			if (cursor.loadFromSystem(sf::Cursor::Arrow))
				window->setMouseCursor(cursor);
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
				newWindowPosition.y = lastWindowPosition.y - (window->getSize().y - lastWindowSize.y);
			}

			if (resizingXLeft)
			{
				newWindowSize.x = lastWindowSize.x - sf::Mouse::getPosition().x + dragOffsetX;
				newWindowPosition.x = lastWindowPosition.x - (window->getSize().x - lastWindowSize.x);
			}

			window->setSize(newWindowSize);
			windowSize = sf::Vector2f(window->getSize());

			window->setPosition(newWindowPosition);
		}
	}

	if (grabbedWindow)
		window->setPosition(sf::Mouse::getPosition() + grabbedOffset);
}

void VGUI::Draw()
{
	window->draw(titlebar);
	window->draw(top);
	window->draw(title);
	window->draw(left);
	window->draw(right);

	window->draw(topLower);
	window->draw(topUpper);
	window->draw(rightLeftBorder);
	window->draw(leftRightBorder);

	window->draw(bottom);
	window->draw(leftLeftBorder);
	window->draw(rightRightBorder);

	window->draw(bottomUpperBorder);
	window->draw(bottomLowerBorder);

	window->draw(resizer);

	window->draw(titleBarButton);
}

void VGUI::updateWindowDecorations()
{
	title.setPosition(sf::Vector2f(sizes.left, (sizes.left / 2)));

	titlebar.setSize(sf::Vector2f(windowSize.x - (sizes.left + sizes.right), sizes.titlebar));
	titlebar.setPosition(sf::Vector2f(sizes.left, sizes.top));

	top.setSize(sf::Vector2f(windowSize.x, sizes.top));

	topUpper.setSize(sf::Vector2f(windowSize.x + ((sizes.left + sizes.right) + (sizes.leftOutline + sizes.rightOutline)), sizes.topOutline));

	topLower.setSize(sf::Vector2f(windowSize.x - (sizes.left + sizes.right), sizes.topOutline));
	topLower.setPosition(sf::Vector2f(sizes.left, sizes.top + sizes.titlebar - sizes.topOutline));

	left.setSize(sf::Vector2f(sizes.left, windowSize.y));

	leftLeftBorder.setSize(sf::Vector2f(sizes.leftOutline, left.getSize().y));

	leftRightBorder.setSize(sf::Vector2f(sizes.leftOutline, left.getSize().y - sizes.bottom- sizes.titlebar - sizes.left + sizes.leftOutline));
	leftRightBorder.setPosition(sf::Vector2f(sizes.left - sizes.leftOutline, sizes.titlebar + sizes.left - sizes.leftOutline));

	right.setSize(sf::Vector2f(sizes.right, windowSize.y));
	right.setPosition(sf::Vector2f(windowSize.x - sizes.right, 0));

	rightLeftBorder.setSize(sf::Vector2f(sizes.rightOutline, right.getSize().y - sizes.bottom - sizes.titlebar - sizes.right + sizes.rightOutline));
	rightLeftBorder.setPosition(sf::Vector2f(windowSize.x - sizes.right, sizes.titlebar + sizes.right - sizes.rightOutline));

	rightRightBorder.setSize(sf::Vector2f(sizes.rightOutline, right.getSize().y));
	rightRightBorder.setPosition(sf::Vector2f(windowSize.x - sizes.rightOutline, 0));

	bottom.setSize(sf::Vector2f(windowSize.x + (sizes.left + sizes.right), sizes.bottom));
	bottom.setPosition(sf::Vector2f(0, windowSize.y - sizes.bottom));

	// TODO: replace bottom with setting sizes.bottom to 80
	bottomUpperBorder.setSize(sf::Vector2f(windowSize.x - (sizes.left + sizes.right) + (sizes.leftOutline + sizes.rightOutline), sizes.bottomOutline));
	bottomUpperBorder.setPosition(sf::Vector2f(sizes.left - sizes.leftOutline, windowSize.y - sizes.bottom));

	bottomLowerBorder.setSize(sf::Vector2f(windowSize.x + ((sizes.left + sizes.right) + (sizes.leftOutline + sizes.rightOutline)), sizes.bottomOutline));
	bottomLowerBorder.setPosition(sf::Vector2f(0, windowSize.y - sizes.bottomOutline));

	resizer.setPosition(sf::Vector2f(window->getSize().x - resizer.getSize().y - 4, window->getSize().y - resizer.getSize().y - 4));
}

void VGUI::initialise()
{
	titlebar.setFillColor(colors.border);
	top.setFillColor(colors.border);
	topUpper.setFillColor(colors.borderWhite);
	topLower.setFillColor(colors.borderBlack);
	left.setFillColor(colors.border);
	leftLeftBorder.setFillColor(colors.borderWhite);
	leftRightBorder.setFillColor(colors.borderBlack);
	right.setFillColor(colors.border);
	rightLeftBorder.setFillColor(colors.borderWhite);
	rightRightBorder.setFillColor(colors.borderBlack);
	bottom.setFillColor(colors.border);
	bottomUpperBorder.setFillColor(colors.borderWhite);
	bottomLowerBorder.setFillColor(colors.borderBlack);

	resizerTexture.loadFromFile("resources/interface/resizer.png");
	resizer.setSize(sf::Vector2f(resizerTexture.getSize().x, resizerTexture.getSize().x));
	resizer.setTexture(&resizerTexture);

	resizer.setPosition(sf::Vector2f(window->getSize() - sf::Vector2u(sizes.bottom, sizes.right)));

	title.setFont(SFUI::Theme::getFont());
	title.setCharacterSize(16);

	updateWindowDecorations();

	titleBarButton.bindToInterface(this);
}
