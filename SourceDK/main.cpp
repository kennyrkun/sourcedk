#include <SFML/Graphics.hpp>

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>

#include "DisabledButton.hpp"

#include "Windows.hpp"
#include "Games.hpp"
#include "Engine.hpp"

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

class WindowDecorations
{
public:
	WindowDecorations()
	{
		// nothing
	}

	~WindowDecorations()
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

void buildHomeInterface(SFUI::Menu& menu)
{
	menu.setPosition(sf::Vector2f(borderWidth + 10, borderWidth + titlebarHeight + 8));

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
	menu.addButton("Application Settings", APPLICATION_SETTINGS);

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

	WindowDecorations decorations;
	sf::RenderWindow& window = decorations.createWindow();
	SFUI::Theme::windowBgColor = decorations.colors.WINDOW_BACKGROUND;

	sf::RectangleShape lowerBorderExtension;
	lowerBorderExtension.setSize(sf::Vector2f(decorations.windowSize.x - (borderWidth * 2), BOTTOM));
	lowerBorderExtension.setPosition(sf::Vector2f(borderWidth, decorations.windowSize.y - BOTTOM));

	SFUI::Menu menu(window);
	menu.setPosition(sf::Vector2f(borderWidth + 10, borderWidth + titlebarHeight + 8));

	buildHomeInterface(menu);

	sf::RectangleShape background;
	background.setSize(sf::Vector2f(window.getSize().x - decorations.right.getSize().x - decorations.left.getSize().x, 80 - decorations.bottom.getSize().y));
	background.setPosition(sf::Vector2f(decorations.bottomUpperBorder.getPosition().x + 1, decorations.bottomUpperBorder.getPosition().y + 1));
	background.setFillColor(decorations.colors.BORDER);

	while (window.isOpen())
	{
		bool mouseOverCloseButton = false;

		sf::Event event;
		while (window.pollEvent(event))
		{
			decorations.HandleEvent(event);

			if (event.type == sf::Event::EventType::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::EventType::Resized)
			{
				background.setSize(sf::Vector2f(event.size.width - (borderWidth * 2), background.getSize().y));
				background.setPosition(decorations.bottom.getPosition() - sf::Vector2f(-borderWidth, background.getSize().y));
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

		window.clear(SFUI::Theme::windowBgColor);

		window.draw(background);

		window.draw(menu);

		// should always be the last thing that gets drawn
		decorations.Draw();

		window.display();
	}

	return EXIT_SUCCESS;
}
