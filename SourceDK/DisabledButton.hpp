#ifndef DISABLED_BUTTON_HPP
#define DISABLED_BUTTON_HPP

#include <SFUI/Widget.hpp>
#include <SFUI/Utils/ItemBox.hpp>

// FIXME: if you click a button, then drag and release elsewhere, 
// and click again without moving the mouse after releasing it will trigger a button press.

// Simple press button.
// A callback is triggered when the button is activated.
class DisabledButton : public SFUI::Widget
{
public:
	DisabledButton(const sf::String& string);

	// Set the displayed button label
	void setString(const sf::String& string);
	// Return the displayed button label.
	const sf::String& getString() const;

	// callbacks 
	void onStateChanged(SFUI::State state);
	void onMouseMoved(float x, float y);
	void onMouseReleased(float x, float y);
	void onKeyPressed(sf::Keyboard::Key key);
	void onKeyReleased(sf::Keyboard::Key key);

	void disable() { disabled = true; }
	void enable() { disabled = false; }

	bool isDisabled() { return disabled; }

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	SFUI::ItemBox<sf::Text> m_box;

	bool disabled = true;
};

#endif // !SFUI_BUTTON_HPP
