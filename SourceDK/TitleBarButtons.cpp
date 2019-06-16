#include"TitleBarButtons.hpp"

#include "VGUI.hpp"

TitleBarButtons::TitleBarButtons(VGUI* interface_)
{
	interface = interface_;
	build();
}

TitleBarButtons::~TitleBarButtons()
{
	delete closeMenu;
}

VGUI* TitleBarButtons::bindToInterface(VGUI* interface_)
{
	interface = interface_;
	build();

	return interface;
}

void TitleBarButtons::build()
{
	closeMenu = new SFUI::Menu(*interface->getWindow());

	SFUI::HorizontalBoxLayout* closehbox = closeMenu->addHorizontalBoxLayout();

	minimiseButtonTexture.loadFromFile("resources/interface/minimise.png");
	minimiseButton = new SFUI::SpriteButton(minimiseButtonTexture);
	closehbox->add(minimiseButton, Callbacks::Minimised);

	/*
	// does this application really need to be maximised?
	maximiseButtonTexture.loadFromFile("resources/interface/maximise.png");
	maximiseButton = new SFUI::SpriteButton(maximiseButtonTexture);
	closehbox->add(maximiseButton, Callbacks::Maximised);
	*/

	closeButtonTexture.loadFromFile("resources/interface/close.png");
	closeButton = new SFUI::SpriteButton(closeButtonTexture);
	closehbox->add(closeButton, Callbacks::Closed);

	closeMenu->setPosition(sf::Vector2f(interface->windowSize.x - closeMenu->getSize().x - interface->sizes.right, interface->sizes.top - 2.0f));
	menuBounds = { closeMenu->getAbsolutePosition(), closeMenu->getSize() };
}

void TitleBarButtons::onEvent(const sf::Event& event)
{
	int id = closeMenu->onEvent(event);

	switch (id)
	{
	case Callbacks::Closed:
		interface->getWindow()->close();
		break;
	case Callbacks::Maximised:
	{
		if (interface->isMaximised)
		{
			windows::window::normal(interface->getWindow()->getSystemHandle());
			maximiseButtonTexture.loadFromFile("resources/interface/maximise.png");
		}
		else
		{
			windows::window::maximise(interface->getWindow()->getSystemHandle());
			maximiseButtonTexture.loadFromFile("resources/interface/unmaximise.png");
		}

		interface->isMaximised = !interface->isMaximised;
		break;
	}
	case Callbacks::Minimised:
	{
		windows::window::minimise(interface->getWindow()->getSystemHandle());
		interface->isMaximised = false;
		break;
	}
	};
}

void TitleBarButtons::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*closeMenu, states);
}
