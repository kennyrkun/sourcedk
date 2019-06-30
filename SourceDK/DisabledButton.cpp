#include "DisabledButton.hpp"

#include <SFUI/Theme.hpp>

DisabledButton::DisabledButton(const sf::String& string)
{
	m_box.item().setFont(SFUI::Theme::getFont());
	m_box.item().setCharacterSize(SFUI::Theme::textCharacterSize);
	m_box.setSize(SFUI::Theme::minWidgetWidth, SFUI::Theme::getBoxHeight());
	setString(string);
	setSize(m_box.getSize());

	hoverText.setFont(SFUI::Theme::getFont());
	hoverText.setCharacterSize(12);
	hoverText.setFillColor(SFUI::Theme::click.textColor);
	hoverText.setString("Not yet available.");

	hoverBox.setSize(sf::Vector2f(hoverText.getGlobalBounds().width + 10, hoverText.getGlobalBounds().height + 10));
	hoverBox.setFillColor(sf::Color(46, 51, 40));

	hoverBox.setPosition(0, -42);
	hoverText.setPosition(4, -38);
}

void DisabledButton::setString(const sf::String& string)
{
	m_box.item().setString(string);

	// Recompute widget width
	float fittingWidth = m_box.item().getLocalBounds().width + SFUI::Theme::PADDING * 2 + SFUI::Theme::borderSize * 2;
	int width = std::max(fittingWidth, SFUI::Theme::minWidgetWidth);

	m_box.setSize(width, SFUI::Theme::getBoxHeight());
	m_box.centerText(m_box.item());

	setSize(m_box.getSize());
}

const sf::String& DisabledButton::getString() const
{
	return m_box.item().getString();
}

void DisabledButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);

	if (disabled)
	{
		sf::RectangleShape s(m_box.getSize());
		s.setFillColor(sf::Color(70, 80, 60, 150));
		target.draw(s, states);

		if (hovered)
		{
			target.draw(hoverBox, states);

			sf::ConvexShape shape;
			shape.setPointCount(3);
			shape.setPoint(0, sf::Vector2f(0, 0));
			shape.setPoint(1, sf::Vector2f(20, 0));
			shape.setPoint(2, sf::Vector2f(10, 15));
			shape.setPosition((m_box.getSize().x / 2) - (shape.getGlobalBounds().width / 2), -20);
			shape.setFillColor(sf::Color(46, 51, 40));
			target.draw(shape, states);

			target.draw(hoverText, states);
		}
	}
}

// Callbacks

void DisabledButton::onStateChanged(SFUI::State state)
{
	if (disabled)
	{
		if (state == SFUI::State::Hovered)
			hovered = true;
		else
			hovered = false;

		return;
	}

	m_box.applyState(state);
}

void DisabledButton::onMouseMoved(float x, float y)
{
	if (disabled)
		return;

	if (getState() == SFUI::State::Pressed)
	{
		if (containsPoint(sf::Vector2f(x, y)))
			m_box.press();
		else
			m_box.release();
	}
}

void DisabledButton::onMouseReleased(float x, float y)
{
	if (disabled)
		return;

	if (containsPoint(sf::Vector2f(x, y)))
		triggerCallback();
}

void DisabledButton::onKeyPressed(sf::Keyboard::Key key)
{
	if (disabled)
		return;

	if (key == sf::Keyboard::Return)
	{
		triggerCallback();
		m_box.press();
	}
}

void DisabledButton::onKeyReleased(sf::Keyboard::Key key)
{
	if (disabled)
		return;

	if (key == sf::Keyboard::Return)
		m_box.release();
}
