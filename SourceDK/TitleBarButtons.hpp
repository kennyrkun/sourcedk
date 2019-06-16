#ifndef TITLE_BAR_BUTTONS_HPP
#define TITLE_BAR_BUTTONS_HPP

#include <SFUI/SFUI.hpp>
#include <SFML/Graphics.hpp>

class VGUI;

class TitleBarButtons : public sf::Drawable
{
public:
	TitleBarButtons()
	{
		// TODO: remove this
	}

	TitleBarButtons(VGUI* interface);

	~TitleBarButtons();

	VGUI* bindToInterface(VGUI* interface);

	SFUI::Menu* closeMenu;
	sf::FloatRect menuBounds;


	SFUI::SpriteButton* minimiseButton;
	SFUI::SpriteButton* maximiseButton;
	SFUI::SpriteButton* closeButton;

	sf::Texture minimiseButtonTexture;
	sf::Texture maximiseButtonTexture;
	sf::Texture closeButtonTexture;

	void onEvent(const sf::Event& event);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	VGUI* interface;

	void build();

	enum Callbacks
	{
		Closed,
		Minimised,
		Maximised
	};
};

#endif // !TITLE_BAR_BUTTONS_HPP
