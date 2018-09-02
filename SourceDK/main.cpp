#include <SFML/Graphics.hpp>

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>

/*
class CustomBorderWindow : public sf::Window, public sf::RenderTarget
{
public:
	CustomBorderWindow()
	{
		// nothing to do
	}

	CustomBorderWindow(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings())
	{
		// Don't call the base class constructor because it contains virtual function calls
		create(mode, title, style, settings);
	}

	explicit CustomBorderWindow(sf::WindowHandle handle, const sf::ContextSettings& settings = sf::ContextSettings())
	{
		// Don't call the base class constructor because it contains virtual function calls
		create(handle, settings);
	}

	virtual ~CustomBorderWindow()
	{
		// nothing to do
	}

	virtual sf::Vector2u getSize() const
	{
		return Window::getSize();
	}

	bool setActive(bool active = true)
	{
		bool result = Window::setActive(active);

		// Update RenderTarget tracking
		if (result)
			RenderTarget::setActive(active);

		// If FBOs are available, make sure none are bound when we
		// try to draw to the default framebuffer of the RenderWindow
		if (active && result && priv::RenderTextureImplFBO::isAvailable())
		{
			priv::RenderTextureImplFBO::unbind();

			return true;
		}

		return result;
	}

protected:
	virtual void onCreate()
	{
		// Just initialize the render target part
		RenderTarget::initialize();
	}

	virtual void onResize()
	{
		// Update the current view (recompute the viewport, which is stored in relative coordinates)
		setView(getView());
	}
}
*/

const int borderWidth(8);
const int titlebarHeight(31 - borderWidth);
const int BOTTOM = 80;

sf::RectangleShape titlebar,
	top,
	topUpper, topLower,
	left,
	leftLeftBorder, leftRightBorder,
	right,
	rightLeftBorder, rightRightBorder,
	bottom,
	bottomUpperBorder, bottomLowerBorder;

sf::RectangleShape gameTypeShape;

sf::Vector2f windowSize = { 310, 490 };
sf::Vector2f useableWindowSize = { windowSize.x - (borderWidth * 2), windowSize.y - (borderWidth * 2) - titlebarHeight };

sf::Text title;

sf::Color WHITE_BORDER = SFUI::Theme::hexToRgb("#889180");
sf::Color BLACK_BORDER = SFUI::Theme::hexToRgb("#282E22");

sf::Color BORDER = SFUI::Theme::hexToRgb("#4C5844");
sf::Color WINDOW_BACKGROUND = SFUI::Theme::hexToRgb("#3E4637");

void updateWindowDecorations()
{
	title.setString("Source SDK");
	title.setFont(SFUI::Theme::getFont());
	title.setCharacterSize(16);
	title.setPosition(sf::Vector2f(borderWidth, (borderWidth / 2)));

	titlebar.setSize(sf::Vector2f(windowSize.x - borderWidth, titlebarHeight));
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
	bottom.move(sf::Vector2f(0, windowSize.y - borderWidth));

	bottomUpperBorder.setSize(sf::Vector2f(windowSize.x - (borderWidth * 2) + 2, 1));
	bottomUpperBorder.setPosition(sf::Vector2f(borderWidth - 1, windowSize.y - BOTTOM));
	
	bottomLowerBorder.setSize(sf::Vector2f(windowSize.x + (borderWidth * 2), 1));
	bottomLowerBorder.setPosition(sf::Vector2f(0, windowSize.y - 1));

	gameTypeShape.setSize(sf::Vector2f(windowSize.x - (borderWidth * 2), BOTTOM));
	gameTypeShape.setPosition(sf::Vector2f(borderWidth, windowSize.y - BOTTOM));
}

int main()
{
	enum CALLBACKS
	{
		MINIMISE,
		QUIT
	};

	SFUI::Theme::loadFont("resources/interface/tahoma.ttf");
	SFUI::Theme::loadTexture("resources/interface/vgui.png");
	SFUI::Theme::textCharacterSize = 11;
	SFUI::Theme::click.textColor = sf::Color::White;
	SFUI::Theme::click.textColorHover = sf::Color::White;
	SFUI::Theme::click.textColorFocus = sf::Color::White;
	SFUI::Theme::input.textColor = sf::Color::White;
	SFUI::Theme::input.textColorHover = sf::Color::White;
	SFUI::Theme::input.textColorFocus = sf::Color::White;
	SFUI::Theme::windowBgColor = SFUI::Theme::hexToRgb("#3E4637");
	SFUI::Theme::PADDING = 2.f;

	// TODO: borders are totalling to 9 

	titlebar.setFillColor(BORDER);
	top.setFillColor(BORDER);
	topUpper.setFillColor(WHITE_BORDER);
	topLower.setFillColor(BLACK_BORDER);
	left.setFillColor(BORDER);
	leftLeftBorder.setFillColor(WHITE_BORDER);
	leftRightBorder.setFillColor(BLACK_BORDER);
	right.setFillColor(BORDER);
	rightLeftBorder.setFillColor(WHITE_BORDER);
	rightRightBorder.setFillColor(BLACK_BORDER);
	bottom.setFillColor(BORDER);
	bottomUpperBorder.setFillColor(WHITE_BORDER);
	bottomLowerBorder.setFillColor(BLACK_BORDER);
	gameTypeShape.setFillColor(BORDER);

	updateWindowDecorations();

//	useableWindowSize.x += borderWidth * 2;
//	useableWindowSize.y += (borderWidth * 2) + titlebarHeight;

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFUI 2", sf::Style::None);
//	sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), title.getString(), sf::Style::None);
	window.setVerticalSyncEnabled(true);

	SFUI::Menu menu(window);
	menu.setPosition(sf::Vector2f(borderWidth + 10, borderWidth + titlebarHeight + 10));

	menu.addLabel("Applications");
	menu.addButton("Hammer Editor");
	menu.addButton("Model Viewer");
	menu.addButton("Face Poser");
	menu.addButton("itemtest");
	menu.addLabel("Documentation");
	menu.addButton("SDK Release Notes");
	menu.addButton("SDK Documentation");
	menu.addLabel("Utilities");
	menu.addButton("Create a Mod");
	menu.addButton("Refresh SDK Content");
	menu.addButton("Reset Game Configurations");
	menu.addButton("Edit Game Configurations");

	SFUI::FormLayout* form2 = menu.addFormLayout();

	enum class Engine
	{
		Gold,
		Source2003,
		Source2006,
		Source2007,
		Source2009,
		Source2013,
		Source2,
	};

	SFUI::ComboBox<Engine>* engineVersion = new SFUI::ComboBox<Engine>(&window);
	engineVersion->addItem("GoldSrc", Engine::Gold);
	engineVersion->addItem("2003", Engine::Source2003);
	engineVersion->addItem("2006", Engine::Source2006);
	engineVersion->addItem("2007", Engine::Source2007);
	engineVersion->addItem("2009", Engine::Source2009);
	engineVersion->addItem("2013", Engine::Source2013);
	engineVersion->addItem("Source2", Engine::Source2);
	form2->addRow("Engine: " , engineVersion, 1336);

	enum class Game
	{
		CSGO,
		CSS,
		CS,

		HL,
		HLS,
		HLOS,
		HLBS,
		HLDM, // Deathmatch Classic
		HLDMS,
		
		HL2,
		HL2E1,
		HL2E2,
		HL2LC,
		HL2DM,

		DOTA2,
		TF2,
		L4D,
		L4D2,

		RICOCHET,
	};

	SFUI::ComboBox<Game>* gameCombo = new SFUI::ComboBox<Game>(&window);
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

//	form2->setPosition(sf::Vector2f(0, 10));

	SFUI::Menu closeMenu(window);
	SFUI::HorizontalBoxLayout* closehbox = closeMenu.addHorizontalBoxLayout();

	/*
	sf::Texture minimiseButtonTexture;
	minimiseButtonTexture.loadFromFile("resources/interface/minimise.png");
	SFUI::SpriteButton* minimiseButton = new SFUI::SpriteButton(minimiseButtonTexture);
	closehbox->add(minimiseButton, CALLBACKS::MINIMISE);
	*/

	sf::Texture closeButtonTexture;
	closeButtonTexture.loadFromFile("resources/interface/close.png");
	SFUI::SpriteButton* closeButton = new SFUI::SpriteButton(closeButtonTexture);
	closehbox->add(closeButton, CALLBACKS::QUIT);
	closeMenu.setPosition(sf::Vector2f(windowSize.x - closeMenu.getSize().x - borderWidth, borderWidth - 2));

	sf::Vector2i grabbedOffset;
	bool grabbedWindow = false;
	bool resizingXYTopLeft = false;
	bool resizingXYTopRight = false;
	bool resizingXYBottomLeft = false;
	bool resizingXYBottomRight = false;
	bool resizingXLeft = false;
	bool resizingXRight = false;
	bool resizingYTop = false;
	bool resizingYBottom = false;

	sf::Vector2u lastWindowSize;
	sf::Vector2u lastWindowSize2;
	int dragOffset;

	// Start the game loop
	while (window.isOpen())
	{
		bool mouseOverCloseButton = false;

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::EventType::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));

				updateWindowDecorations();

				closeMenu.setPosition(sf::Vector2f(windowSize.x - closeMenu.getSize().x - borderWidth, borderWidth - 2));
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::FloatRect clsbtn(closeButton->getAbsolutePosition(), closeButton->getSize());
					if (clsbtn.contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					{
						mouseOverCloseButton = true;
					}
					else
					{
						// TODO: make this not suck
						closeButton->onStateChanged(SFUI::State::Default);

						// TODO: don't drag if over closeMenu
						if (titlebar.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
							grabbedWindow = true;
						else if (top.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
							resizingYTop = true;
						else if (left.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
							resizingXLeft = true;
						else if (right.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
							resizingXRight = true;
						else if (bottom.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
							resizingYBottom = true;

						grabbedOffset = window.getPosition() - sf::Mouse::getPosition();
						dragOffset = sf::Mouse::getPosition().x;

						lastWindowSize = window.getSize();
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					// only run this here so it only triggers if the mouse is released
					if (!grabbedWindow)
					{
						int id = closeMenu.onEvent(event);
						switch (id)
						{
						case CALLBACKS::QUIT:
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
				}
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				// TODO: optimise this so it's not dog shit

				sf::FloatRect clsbtn(closeButton->getAbsolutePosition(), closeButton->getSize());
				if (!clsbtn.contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
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
				}
				else
				{
					sf::Cursor cursor;
					if (cursor.loadFromSystem(sf::Cursor::Arrow))
						window.setMouseCursor(cursor);
				}

				if (resizingXRight)
				{
					window.setSize(sf::Vector2u(lastWindowSize.x + sf::Mouse::getPosition().x - dragOffset, lastWindowSize.y));

					windowSize = sf::Vector2f(window.getSize());
				}
			}

			int id = menu.onEvent(event);
			switch (id)
			{
				// no events
			};

			int isd = closeMenu.onEvent(event);
			switch (isd)
			{
			case CALLBACKS::QUIT:
				if (mouseOverCloseButton)
					window.close();
				break;
			};
		}

		if (grabbedWindow)
			window.setPosition(sf::Mouse::getPosition() + grabbedOffset);

		window.clear(SFUI::Theme::windowBgColor);

		window.draw(gameTypeShape);

		window.draw(menu);

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

		window.draw(closeMenu);

		window.display();
	}

	return EXIT_SUCCESS;
}
