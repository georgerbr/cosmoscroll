#ifndef GUI_PROGRESSBAR_HPP
#define GUI_PROGRESSBAR_HPP

#include "Widget.hpp"

namespace gui
{

/**
 * Une barre de progression
 */
class ProgressBar: public Widget
{
public:
	/**
	 * @param owner: menu propriétaire de la barre de progression
	 * @param x: position x
	 * @param y: position y
	 * @param w: largeur
	 * @param h: hauteur
	 */
	ProgressBar(Menu* owner, int x, int y, int w, int h);

	void SetValue(int value);

	int GetValue() const;

private:
	void Render(sf::RenderTarget& target) const;

	sf::Shape background_;
	sf::Sprite bar_;
	int value_;
};

}

#endif // GUI_PROGRESSBAR_HPP