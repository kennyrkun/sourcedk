#include "SFUI.hpp"
#include "Theme.hpp"

// This is an example program uitlising every control in SFUI.
// It is intended to be reviewed, and used as a learning tool
// as well as quickly previewing any changes made SFUI itself.

// Resources are not provided twice here, if you are missing
// resources copy the or symlink the folder in SFUI.

int main()
{
	enum Callbacks
	{
		TEXT,
		COLOR,
		ROTATION,
		BOLD,
		UNDERLINED,
		SCALE,
		NEW_BUTTON,
		VSLIDER,
		THEME_TEXTURE,
		TERMINATE,
		QUIT
	};

	bool doTerminateProgram(false);

	do
	{
//		sf::RenderWindow window(sf::VideoMode(640, 480), "SFUI 2", sf::Style::Titlebar | sf::Style::Close);
		sf::RenderWindow window(sf::VideoMode(640, 480), "SFUI 2", sf::Style::None);

		SFUI::Menu menu(window);
//		menu.setPosition(sf::Vector2f(10, 10));

		SFUI::Theme::loadFont("resources/interface/tahoma.ttf");
		SFUI::Theme::loadTexture("resources/interface/square.png");
		SFUI::Theme::textCharacterSize = 11;
		SFUI::Theme::click.textColor = SFUI::Theme::hexToRgb("#191B18");
		SFUI::Theme::click.textColorHover = SFUI::Theme::hexToRgb("#191B18");
		SFUI::Theme::click.textColorFocus = SFUI::Theme::hexToRgb("#000000");
		SFUI::Theme::input.textColor = SFUI::Theme::hexToRgb("#000000");
		SFUI::Theme::input.textColorHover = SFUI::Theme::hexToRgb("#CC7A00");
		SFUI::Theme::input.textColorFocus = SFUI::Theme::hexToRgb("#000000");
		SFUI::Theme::windowBgColor = SFUI::Theme::hexToRgb("#dddbde");
		SFUI::Theme::PADDING = 2.f;

		SFUI::HorizontalBoxLayout* hbox = menu.addHorizontalBoxLayout();
		SFUI::FormLayout* form = hbox->addFormLayout();
		SFUI::VerticalBoxLayout* vbox = hbox->addVerticalBoxLayout();
		SFUI::HorizontalBoxLayout* hbox2 = vbox->addHorizontalBoxLayout();
		SFUI::HorizontalBoxLayout* hbox3 = vbox->addHorizontalBoxLayout();

		// Textbox
		SFUI::InputBox* textbox = new SFUI::InputBox();
		textbox->setText("Hello, World!");
		form->addRow("Text", textbox, Callbacks::TEXT);

		// Slider for rotation
		SFUI::Slider* sliderRotation = new SFUI::Slider();
		sliderRotation->setQuantum(1);
		form->addRow("Rotation", sliderRotation, Callbacks::ROTATION);

		// Progress bar
		SFUI::ProgressBar* pbar0 = new SFUI::ProgressBar();
		form->add(pbar0);

		// Slider for scale
		SFUI::Slider* sliderScale = new SFUI::Slider();
		form->addRow("Scale", sliderScale, Callbacks::SCALE);

		// OptionsBox for color
		SFUI::OptionsBox<sf::Color>* opt = new SFUI::OptionsBox<sf::Color>();
		opt->addItem("Red", sf::Color::Red);
		opt->addItem("Blue", sf::Color::Blue);
		opt->addItem("Green", sf::Color::Green);
		opt->addItem("Yellow", sf::Color::Yellow);
		opt->addItem("White", sf::Color::White);
		form->addRow("Color", opt, Callbacks::COLOR);

		// OptionsBox for color
		SFUI::ComboBox<sf::Color>* optcb = new SFUI::ComboBox<sf::Color>(&window);
		optcb->addItem("Red", sf::Color::Red);
		optcb->addItem("Blue", sf::Color::Blue);
		optcb->addItem("Green", sf::Color::Green);
		optcb->addItem("Yellow", sf::Color::Yellow);
		optcb->addItem("White", sf::Color::White);
		form->addRow("Color", optcb);

		// Checbkox
		SFUI::CheckBox* checkboxBold = new SFUI::CheckBox();
		form->addRow("Bold text", checkboxBold, Callbacks::BOLD);

		SFUI::CheckBox* checkboxUnderlined = new SFUI::CheckBox();
		form->addRow("Underlined text", checkboxUnderlined, Callbacks::UNDERLINED);

		// Custom button
		sf::Texture spriteButton_texture;
		spriteButton_texture.loadFromFile("resources/interface/themed-button.png");

		SFUI::SpriteButton* customButton = new SFUI::SpriteButton(spriteButton_texture, "Custom Button");

		customButton->setTextSize(18);
		form->add(customButton);

		// Textbox
		SFUI::InputBox* changeTextureBox = new SFUI::InputBox();
		changeTextureBox->setText("resources/interface/texture_square.png");
		form->addRow("Theme Texture", changeTextureBox, Callbacks::THEME_TEXTURE);

		vbox->addLabel("This panel is on the left");

		// Textbox
		SFUI::InputBox* textbox2 = new SFUI::InputBox(100);
		textbox2->setText("Button name");
		hbox2->add(textbox2, Callbacks::NEW_BUTTON);
		hbox2->addButton("Create button", Callbacks::NEW_BUTTON);

		// Small progress bar
		hbox3->addLabel("Small progress bar");
		SFUI::ProgressBar* pbar = new SFUI::ProgressBar(40);
		hbox3->add(pbar);

		SFUI::Slider* vslider = new SFUI::Slider(100, SFUI::Slider::Type::Vertical);
		hbox->add(vslider, Callbacks::VSLIDER);

		SFUI::CheckBox* terminateProgram = new SFUI::CheckBox();
		form->addRow("Terminate Program?", terminateProgram, Callbacks::TERMINATE);

		menu.addButton("Quit", Callbacks::QUIT);

		sf::Text text(textbox->getText(), SFUI::Theme::getFont());
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
		text.setPosition(window.getSize().x / 2, 400);

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				int id = menu.onEvent(event);
				switch (id)
				{
				case Callbacks::TEXT:
					text.setString(textbox->getText());
					text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
					break;
				case Callbacks::COLOR:
					text.setFillColor(opt->getSelectedValue());
					break;
				case Callbacks::ROTATION:
					text.setRotation(sliderRotation->getValue() * 360 / 100.f);
					pbar0->setValue(sliderRotation->getValue());
					break;
				case Callbacks::UNDERLINED:
				{
					int style = text.getStyle();
					if (checkboxUnderlined->isChecked())
						style |= sf::Text::Underlined;
					else
						style &= ~sf::Text::Underlined;
					text.setStyle(style);
					break;
				}
				case Callbacks::BOLD:
				{
					int style = text.getStyle();
					if (checkboxBold->isChecked())
						style |= sf::Text::Bold;
					else
						style &= ~sf::Text::Bold;
					text.setStyle(style);
					break;
				}
				case Callbacks::SCALE:
				{
					float scale = 1 + sliderScale->getValue() * 2 / 100.f;
					text.setScale(scale, scale);
					break;
				}
				case Callbacks::QUIT:
					window.close();
					break;
				case Callbacks::VSLIDER:
					pbar->setValue(vslider->getValue());
					break;
				case Callbacks::NEW_BUTTON:
					vbox->addButton(textbox2->getText());
					break;
				case Callbacks::THEME_TEXTURE:
					SFUI::Theme::loadTexture(changeTextureBox->getText());
				case Callbacks::TERMINATE:
					doTerminateProgram = !doTerminateProgram;
				}

				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear(SFUI::Theme::windowBgColor);

			window.draw(menu);
			window.draw(text);

			window.display();
		}
	} while (!doTerminateProgram);

	return EXIT_SUCCESS;
}

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

int smain()
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
